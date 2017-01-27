#include "stdafx.h"
#include "websocket.h"


WebSocket websocket;

void WebSocket::OverlappedDetail::remove()
{
	client->removeOverlapped(this);
}

WebSocket::Client::Client(SOCKET s)
{
	socket = s;
	lock_mutex = CreateMutex(NULL, FALSE, NULL);
	ws_status = ws_status_connecting;
	addr[0] = 0;
	port = 0;
	timestamp.QuadPart = 0;

	websocket.clients.push_back(this);

	it = websocket.clients.end();
	--it;
}

WebSocket::Client::~Client()
{
	if(closesocket(socket) == SOCKET_ERROR)
		websocket.log(L"removeClient - closesocket function failed with error %d\n", WSAGetLastError());

	if(*addr != 0)
	{
		websocket.log(L"client %s:%d disconnected\n", addr, port);

		CallbackMsg cb_msg;
		cb_msg.status = cbs_disconnected;
		cb_msg.data = (void*)this;
		websocket.callbackStatus(&cb_msg);
	}

	websocket.clients.erase(it);

	for (std::list<OverlappedDetail*>::iterator it = overlapped_list.begin(); it != overlapped_list.end(); ++it)
	{
		(*it)->client = NULL;
		(*it)->operation = operation_cancel;
	}
//		 delete (*it);

	CloseHandle(lock_mutex);
}


WebSocket::WebSocket() :
listen_port(80),
acceptEx(NULL),
IOCP_handle(NULL),
num_threads(2),
worker_threads(NULL),
shutdown_event(NULL),
cleanup_flag(0),
callback_func(NULL),
server_status(server_status_stopped)
{
	QueryPerformanceFrequency(&frequency);
}

WebSocket::~WebSocket()
{
}

void WebSocket::log(wchar_t *format, ...)
{
#ifdef _DEBUG

	WCHAR buffer[1024];

	EnterCriticalSection(&console_cs);

	va_list args;
	va_start(args, format);

	_vsnwprintf_s(buffer, sizeof(buffer), format, args);

	va_end(args);

	OutputDebugString(buffer);

	LeaveCriticalSection(&console_cs);

#endif
}

bool WebSocket::initServer(USHORT port, void (*callbackFunc)(CallbackMsg* msg), void* callbackOwner)
{
	server_status = server_status_starting;

	InitializeCriticalSection(&console_cs);
	InitializeCriticalSection(&callback_cs);
	shutdown_event = CreateEvent(NULL, TRUE, FALSE, NULL);

	setCallbackFunc(callbackFunc, callbackOwner);

	WSADATA wsaData;

	int result;
	if((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != NO_ERROR)
	{
		log(L"initServer - WSAStartup() failed with error: %d\n", result);
		return false;
	}

	cleanup_flag |= CLEANUP_FLAG_WSACLEANUP;

	if((listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		log(L"initServer - WSASocket function failed with error: %d\n", WSAGetLastError());
		return false;
	}

	cleanup_flag |= CLEANUP_FLAG_LISTENSOCKET;

	listen_port = port;
	sockaddr_in service;
	memset(&service, 0, sizeof(sockaddr_in));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(listen_port);

	if(bind(listen_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		log(L"initServer - bind function failed with error %d\n", WSAGetLastError());
		return false;
	}
	if(listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		log(L"initServer - listen function failed with error: %d\n", WSAGetLastError());
		return false;
	}

	WCHAR addr_buf[16];
	int addr_len = sizeof(service);
	getsockname(listen_socket, (sockaddr*)&service, &addr_len);
	InetNtop(AF_INET, &service.sin_addr, addr_buf, 16);
	log(L"Server is listening on socket... %s:%u\n", addr_buf, ntohs(service.sin_port));

	CallbackMsg cb_msg;
	cb_msg.status = cbs_listening;
	CallbackAddr cb_addr;
	cb_addr.addr = addr_buf;
	cb_addr.port = ntohs(service.sin_port);
	cb_msg.data = (void*)&cb_addr;
	callbackStatus(&cb_msg);

	if((IOCP_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		log(L"initServer - CreateIoCompletionPort function failed with error %d\n", GetLastError());
		return false;
	}

	if(CreateIoCompletionPort((HANDLE)listen_socket, IOCP_handle, ULONG_PTR(0), 0) == NULL)
	{
		log(L"initServer - CreateIoCompletionPort function failed with error %d\n", GetLastError());
		return false;
	}

	cleanup_flag |= CLEANUP_FLAG_COMPLETIONPORT;

	worker_threads = new HANDLE[num_threads];
	unsigned int thread_id;
	for(int i = 0; i < num_threads; ++i)
		worker_threads[i] = (HANDLE)_beginthreadex(NULL, 0, workerThread, (void*)this, 0, &thread_id);

	cleanup_flag |= CLEANUP_FLAG_WORKERTHREAD;

	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	DWORD bytes_return;
	if(WSAIoctl(listen_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx), &acceptEx, sizeof(acceptEx), &bytes_return, NULL, NULL) == SOCKET_ERROR)
	{
		log(L"initServer - WSAIoctl function failed with error %d\n", WSAGetLastError());
		return false;
	}

	server_status = server_status_started;

	for(int i = 0; i < 5; ++i)
		acceptSocket();

	return true;
}

void WebSocket::cleanup()
{
	if(shutdown_event == NULL)
		return;

	SetEvent(shutdown_event);

	server_status = server_status_stopping;

	if(cleanup_flag & CLEANUP_FLAG_WORKERTHREAD)
	{
		PostQueuedCompletionStatus(IOCP_handle, 0, (DWORD)NULL, NULL);

		WaitForMultipleObjects(num_threads, worker_threads, TRUE, INFINITE);

		for(int i = 0; i < num_threads; ++i)
		{
			CloseHandle(worker_threads[i]);
		}

		delete[] worker_threads;
		worker_threads = NULL;
	}

	while(!clients.empty())
		delete clients.front();
	clients.clear();

	if(cleanup_flag & CLEANUP_FLAG_LISTENSOCKET)
	{
		if(closesocket(listen_socket) == SOCKET_ERROR)
			log(L"cleanup - closesocket function failed with error %d\n", WSAGetLastError());
	}

	if(cleanup_flag & CLEANUP_FLAG_WSACLEANUP)
		WSACleanup();

	if(cleanup_flag & CLEANUP_FLAG_COMPLETIONPORT && IOCP_handle)
	{
		CloseHandle(IOCP_handle);
		IOCP_handle = NULL;
	}

	CloseHandle(shutdown_event);
	shutdown_event = NULL;

	DeleteCriticalSection(&console_cs);

	cleanup_flag = 0;

	server_status = server_status_stopped;

	CallbackMsg cb_msg;
	cb_msg.status = cbs_server_stopped;
	cb_msg.data = NULL;
	callbackStatus(&cb_msg);

	DeleteCriticalSection(&callback_cs);
}

unsigned int __stdcall WebSocket::workerThread(LPVOID lParam)
{
	WebSocket* ws = (WebSocket*)lParam;

	DWORD bytes_transfered = 0;
	ULONG_PTR key = NULL;
	OverlappedDetail* overlapped = NULL;
	CallbackMsg cb_msg;
	Client* client = NULL;

	while(WaitForSingleObject(ws->shutdown_event, 0) != WAIT_OBJECT_0)
	{
		BOOL result = GetQueuedCompletionStatus(ws->IOCP_handle, &bytes_transfered, (PULONG_PTR)&key, (LPOVERLAPPED*)&overlapped, INFINITE);

		if(overlapped == NULL)
		{
			if(result)
			{
				PostQueuedCompletionStatus(ws->IOCP_handle, 0, 0, NULL);
				break;
			}
			else // connection has closed.
				continue;
		}

		if(overlapped->operation == operation_cancel)
			continue;

		DWORD result1;
		if((result1 = WaitForSingleObject(overlapped->client->lock_mutex, INFINITE)) != WAIT_OBJECT_0)
		{
			ws->log(L"workerThread - WaitForSingleObject client->lock_mutex function %d\n", result1);
			continue;
		}

		client = overlapped->client;

		switch(overlapped->operation)
		{
		case operation_accept:
			if(!ws->onAccept(overlapped))
				break;
			break;

		case operation_recv:
			if(bytes_transfered == 0)
			{
				ws->closeClient(client);
				break;
			}

			if(!ws->onRecv(overlapped, bytes_transfered))
				break;
			break;

		case operation_send_handshake:
			client->ws_status = ws_status_approving;

			ws->getElapsedTime(&client->timestamp);

			cb_msg.status = cbs_ws_connected;
			cb_msg.data = (void*)client;
			ws->callbackStatus(&cb_msg);

		case operation_send:
			if(bytes_transfered == 0)
			{
				ws->closeClient(client);
				break;
			}

			if(!ws->onSend(overlapped))
				continue;
			break;

		case operation_disconnect:
			delete client;
			continue;
		}

		ReleaseMutex(client->lock_mutex);
	}

	return 0;
}

bool WebSocket::acceptSocket()
{
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	Client* client = new Client(client_socket);

	if(CreateIoCompletionPort((HANDLE)client_socket, IOCP_handle, ULONG_PTR(0), 0) == NULL)
	{
		log(L"acceptSocket - CreateIoCompletionPort function failed with error %d\n", GetLastError());
		delete client;
		return false;
	}

	OverlappedDetail* overlapped = client->createOverlapped(operation_accept);

	DWORD byes_received = 0;
	if(!acceptEx(listen_socket, client_socket, overlapped->buffer.buf, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &byes_received, (LPOVERLAPPED)overlapped))
	{
		int error_code;
		if((error_code = WSAGetLastError()) != ERROR_IO_PENDING)
		{
			log(L"acceptSocket - acceptEx function failed with error %d\n", error_code);
			delete client;
			return false;
		}
	}

	return true;
}

bool WebSocket::onAccept(OverlappedDetail* overlapped)
{
	acceptSocket();

	if(setsockopt(overlapped->client->socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (const char *)&listen_socket, sizeof(listen_socket)) == SOCKET_ERROR)
	{
		log(L"onAccept - setsockopt function failed with error %d\n", WSAGetLastError());
		closeClient(overlapped->client);
		return true;
	}

	sockaddr_in addr;
	int addr_len = sizeof(addr);
	getpeername(overlapped->client->socket, (sockaddr*)&addr, &addr_len);
	InetNtop(AF_INET, &addr.sin_addr, overlapped->client->addr, 16);
	overlapped->client->port = ntohs(addr.sin_port);
	log(L"Client - %s:%d connected.\n", overlapped->client->addr, overlapped->client->port);

	CallbackMsg cb_msg;
	cb_msg.status = cbs_connected;
	cb_msg.data = (void*)overlapped->client;
	callbackStatus(&cb_msg);

	return recvSocket(overlapped);
//	return true;
}

bool WebSocket::recvSocket(OverlappedDetail* overlapped)
{
	overlapped->reset();
	overlapped->operation = operation_recv;

	DWORD flags = 0;
	if(WSARecv(overlapped->client->socket, &overlapped->buffer, 1, NULL, &flags, (LPWSAOVERLAPPED)overlapped, NULL) == SOCKET_ERROR)
	{
		int error_code = WSAGetLastError();
		if(error_code != WSA_IO_PENDING)
		{
			log(L"recvSocket - WSARecv function failed with error %d\n", error_code);
			closeClient(overlapped->client);
			return false;
		}
	}

	return true;
}

bool WebSocket::onRecv(OverlappedDetail* overlapped, DWORD bytes_recv)
{
	if(bytes_recv > 0)
	{
		char* buffer = overlapped->buffer.buf;
		DWORD len = bytes_recv;
		buffer[bytes_recv] = 0;

		log(L"Data received from %s:%d - \"%S\"\n", overlapped->client->addr, overlapped->client->port, buffer);

		if(!overlapped->client->recv_buffer.empty())
		{
			overlapped->client->recv_buffer.insert(overlapped->client->recv_buffer.end() - 1, buffer, buffer + bytes_recv);
			overlapped->client->recv_buffer.push_back(0);
			buffer = overlapped->client->recv_buffer.data();
			len = overlapped->client->recv_buffer.size() - 1;
		}

		if(!processRecv(overlapped, buffer, len))
			return false;
	}

	return recvSocket(overlapped);

//	return true;
}

bool WebSocket::sendSocket(OverlappedDetail* overlapped)
{
	DWORD bytes_send = overlapped->buffer.len;
	DWORD flags = 0;

	if(WSASend(overlapped->client->socket, &overlapped->buffer, 1, &bytes_send, flags, (LPWSAOVERLAPPED)overlapped, NULL) == SOCKET_ERROR)
	{
		int error_code = WSAGetLastError();
		if(error_code != WSA_IO_PENDING)
		{
			log(L"sendSocket - WSASend function failed with error %d\n", error_code);
			closeClient(overlapped->client);
			return false;
		}
	}

	return true;
}


bool WebSocket::onSend(OverlappedDetail* overlapped)
{
	Client* client = overlapped->client;

	overlapped->remove();

	ReleaseMutex(client->lock_mutex);

	return false;
}

bool WebSocket::processRecv(OverlappedDetail* overlapped, char* buffer, DWORD buflen)
{
	Client* client = overlapped->client;

	if(client->ws_status == ws_status_connecting)
	{
		static char head[] = "GET / HTTP/1.1";
		DWORD len = sizeof(head) - 1;
		if(buflen >= len && strncmp(head, buffer, len) == 0 && *(unsigned long*)(buffer + buflen - 4) == 0x0a0d0a0d)
		{
			static char key[] = "Sec-WebSocket-Key: ";
			len = sizeof(key) - 1;
			char* begin = strstr(buffer, key);
			if(begin == NULL)
				goto pending_recv;

			begin += len;
			char* bufend = buffer + buflen;
			while(!(*begin > 32 && *begin < 127) && *begin != 0)
				++begin;
			if(*begin == 0 || begin + 24 > buffer + buflen)
				goto pending_recv;;

			char code[25];
			int i = 0;
			for(; i < 24 && begin[i] > 32 && begin[i] < 127; ++i)
				code[i] = begin[i];
			if(i != 24) // invaild format
			{
				closeClient(client);
				return false;
			}
			code[24] = 0;

			client->recv_buffer.clear();

			char handshake[29];
			WebSocketHandshake::generate(code, handshake);
			handshake[28] = 0;

			OverlappedDetail* ol = client->createOverlapped(operation_send_handshake);
			ol->buffer.buf = new char[BUFSIZ];
			ol->buffer.len = BUFSIZ;
			sprintf_s(ol->buffer.buf, ol->buffer.len,
				"HTTP/1.1 101 Switching Protocols\r\n"
				"Upgrade: websocket\r\n"
				"Connection: Upgrade\r\n"
				"Sec-WebSocket-Accept: %s\r\n\r\n"
				, handshake);
			ol->buffer.len = strlen(ol->buffer.buf);

			QueryPerformanceCounter(&client->timestamp);

			if(!sendSocket(ol))
				return false;

			client->ws_status = ws_status_handshake_sent;

			CallbackMsg cb_msg;
			cb_msg.status = cbs_ws_connecting;
			cb_msg.data = (void*)client;
			callbackStatus(&cb_msg);

			return true;
		}

	pending_recv:
		if(client->recv_buffer.empty())
			client->recv_buffer.assign(buffer, buffer + buflen);
	}
	else if(client->ws_status == ws_status_connected)
	{
		if(buflen < 6)
			return true;

		unsigned char* ubuf = (unsigned char*)buffer;
		unsigned char fin = ubuf[0] >> 0x7;
		unsigned char opcode = ubuf[0] & 0x0f;
		unsigned char is_mask = ubuf[1] >> 0x7;
		if(!is_mask)
		{
			closeClient(client);
			return false;
		}

		unsigned long long len = ubuf[1] & 0x7f;
		unsigned char mask[4];
		unsigned char* buf;

		if(len <= 125)
		{
			*((unsigned int*)mask) = *(unsigned int*)(ubuf + 2);
			buf = ubuf + 6;
			if(len > buflen - 6)
				return true;
		}
		else if(len == 126)
		{
			len = *(unsigned short*)(ubuf + 2);
			*((unsigned int*)mask) = *(unsigned int*)(ubuf + 4);
			buf = ubuf + 8;
			if(len > buflen - 8)
				return true;
		}
		else
		{
			len = *(unsigned long long*)(ubuf + 2);
			*((unsigned int*)mask) = *(unsigned int*)(ubuf + 10);
			buf = ubuf + 14;
			if(len > buflen - 14)
				return true;
		}

		for(unsigned long long i = 0; i < len; ++i)
			buf[i] ^= mask[i % 4];

#ifdef _DEBUG
		char str[1024];
		unsigned int str_len = (unsigned int)(len < 1023 ? len : 1023);
		memcpy_s(str, 1024, buf, str_len);
		str[str_len] = 0;
		log(L"Data received from %s:%d \"%S\"\n", client->addr, client->port, str);
#endif

		CallbackMsg cb_msg;
		CallbackRecv cb_recv;
		OverlappedDetail* ol;

		switch(opcode)
		{
		case WS_OPCODE_CONTINUATION:	// continuation
		case WS_OPCODE_TEXT:			// text
		case WS_OPCODE_BINARY:			// binary

			cb_msg.status = cbs_ws_recv;
			cb_msg.data = (void*)&cb_recv;
			cb_recv.client = client;
			cb_recv.data = (char*)buf;
			cb_recv.len = len;
			callbackStatus(&cb_msg);
			break;

		case WS_OPCODE_CLOSE:	// close
			closeClient(client);
			return false;

		case WS_OPCODE_PING:	// ping
			if(len <= 125)
			{
				ol = client->createOverlapped(operation_send);
				ol->buffer.len = (unsigned int)(len + 2);
				ol->buffer.buf = new char[ol->buffer.len + 2];
				buffer[0] = (unsigned char)(WS_FRAME_FIN | WS_OPCODE_PONG);
				buffer[1] = 128 | (unsigned char)ol->buffer.len;
				memcpy(ol->buffer.buf + 2, buf, ol->buffer.len);
				if(!sendSocket(ol))
					return false;
			}
			break;

		case WS_OPCODE_PONG:	// pong
			if(client->timestamp.QuadPart > 0)
			{
				getElapsedTime(&client->timestamp);
				cb_msg.status = cbs_ws_pong;
				cb_msg.data = (void*)client;
				callbackStatus(&cb_msg);
				client->timestamp.QuadPart = 0;
			}
			break;
		}
	}
	else if(client->ws_status == ws_status_approving)
	{
		unsigned char opcode = *((unsigned char*)buffer) & 0x0f;
		if(opcode == WS_OPCODE_CLOSE)
		{
			closeClient(client);
			return false;
		}
	}

	return true;
}

void WebSocket::closeClient(Client * client)
{
	if(client->ws_status != ws_status_disconnecting)
	{
		client->ws_status = ws_status_disconnecting;
		OverlappedDetail* ol = client->createOverlapped(operation_disconnect);
		PostQueuedCompletionStatus(IOCP_handle, 0, 0, (LPOVERLAPPED)ol);
	}
}

bool WebSocket::recvMessage(Client * client)
{
	OverlappedDetail* ol = client->createOverlapped(operation_recv);
	return recvSocket(ol);
}

bool WebSocket::sendMessage(const char* msg, DWORD len, Client* client)
{
	char* buffer = NULL;
	DWORD buflen = len - 1;

	if(buflen <= 125)
	{
		buffer = new char[buflen + 2];
		buffer[0] = (unsigned char)(WS_FRAME_FIN | WS_OPCODE_TEXT);
		buffer[1] = (char)buflen;
		memcpy(buffer + 2, msg, buflen);
		buflen += 2;
	}
	else if(buflen <= 65535)
	{
		buffer = new char[buflen + 4];
		buffer[0] = (unsigned char)(WS_FRAME_FIN | WS_OPCODE_TEXT);
		buffer[1] = 126;
		*(WORD*)(buffer + 2) = (WORD)buflen;
		memcpy(buffer + 4, msg, buflen);
		buflen += 4;
	}
	else
	{
		buffer = new char[buflen + 10];
		buffer[0] = (unsigned char)(WS_FRAME_FIN | WS_OPCODE_TEXT);
		buffer[1] = 127;
		*(DWORD*)(buffer + 2) = 0;
		*(DWORD*)(buffer + 6) = buflen;
		memcpy(buffer + 10, msg, buflen);
		buflen += 10;
	}

	if(client == NULL)
	{
		for(std::list<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			client = (*it);

			DWORD result;
			if((result = WaitForSingleObject(client->lock_mutex, INFINITE)) != WAIT_OBJECT_0)
			{
				log(L"sendMessage - WaitForSingleObject client->lock_mutex function %d\n", result);
				continue;
			}

			if(client->ws_status == ws_status_connected)
			{
				OverlappedDetail* ol = client->createOverlapped(operation_send);
				ol->buffer.buf = new char[buflen];
				memcpy(ol->buffer.buf, buffer, buflen);
				ol->buffer.len = buflen;
				sendSocket(ol);
			}

			ReleaseMutex(client->lock_mutex);
		}

		if(buffer)
			delete[] buffer;
	}
	else
	{
		DWORD result;
		if((result = WaitForSingleObject(client->lock_mutex, INFINITE)) != WAIT_OBJECT_0)
		{
			log(L"sendMessage - WaitForSingleObject client->lock_mutex function %d\n", result);
			return true;
		}

		bool result1;
		if(client->ws_status == ws_status_connected)
		{
			OverlappedDetail* ol = client->createOverlapped(operation_send);
			ol->buffer.buf = buffer;
			ol->buffer.len = buflen;
			result1 = sendSocket(ol);
		}

		ReleaseMutex(client->lock_mutex);

		return result1;
	}

	return true;
}

bool WebSocket::ping(Client* client)
{
	if(client == NULL)
	{
		for(std::list<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
			ping(*it);
		return true;
	}

	if(client->ws_status == ws_status_connected)
	{
		OverlappedDetail* ol = client->createOverlapped(operation_send);
		unsigned char len = 3;
		ol->buffer.buf = new char[len];
		ol->buffer.len = len;
		ol->buffer.buf[0] = (unsigned char)(WS_FRAME_FIN | WS_OPCODE_PING);
		ol->buffer.buf[1] = len - 2;
		ol->buffer.buf[2] = 0x31;

		QueryPerformanceCounter(&client->timestamp);

		return sendSocket(ol);
	}

	return true;
}

void WebSocket::setCallbackFunc(void (*callbackFunc)(CallbackMsg *msg), void* callbackOwner)
{
	callback_func = callbackFunc;
	callback_owner = callbackOwner;
}

void WebSocket::callbackStatus(CallbackMsg * msg)
{
	if(callback_func)
	{
		EnterCriticalSection(&callback_cs);

		msg->owner = callback_owner;
		callback_func(msg);

		LeaveCriticalSection(&callback_cs);
	}
}

void WebSocket::getElapsedTime(LARGE_INTEGER* timestamp)
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	time.QuadPart -= timestamp->QuadPart;
	time.QuadPart *= 1000000;
	timestamp->QuadPart = time.QuadPart / frequency.QuadPart;
}