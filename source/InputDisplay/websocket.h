#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinBase.h>

#include <winsock2.h>
#include <Mswsock.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib,"Ws2_32.lib")

#include <windows.h>
#include <process.h>

#include <list>
#include <vector>

#include "libwshandshake.hpp"


class WebSocket
{
public:

	WebSocket();
	~WebSocket();

public:

#define WS_FRAME_FIN 128

#define WS_OPCODE_CONTINUATION 0
#define WS_OPCODE_TEXT         1
#define WS_OPCODE_BINARY       2
#define WS_OPCODE_CLOSE        8
#define WS_OPCODE_PING         9
#define WS_OPCODE_PONG         10

	enum Operation
	{
		operation_accept,
		operation_recv,
		operation_send,
		operation_connect,
		operation_disconnect,
		operation_send_handshake,
		operation_cancel,
	};

	enum WS_Status
	{
		ws_status_connecting,
		ws_status_handshake_sent,
		ws_status_approving,
		ws_status_connected,
		ws_status_disconnecting,
		ws_status_disconnected,
	};

	struct Client;

	struct OverlappedDetail : OVERLAPPED
	{
		Operation operation;
		WSABUF buffer;
		Client* client;
		std::list<OverlappedDetail*>::iterator it;

		OverlappedDetail(Client* cl, Operation op)
		{
			memset(this, 0, sizeof(OverlappedDetail));
			if(op == operation_accept || op == operation_recv)
			{
				buffer.len = BUFSIZ;
				buffer.buf = new CHAR[BUFSIZ + 1];
			}
			operation = op;
			client = cl;
		};

		~OverlappedDetail()
		{
			if(buffer.buf)
				delete[] buffer.buf;
		};

		void reset()
		{
			//		CHAR* buf = buffer.buf;
			//		Client* cl = client;
			memset(this, 0, sizeof(OVERLAPPED));
			//		buffer.buf = buf;
			buffer.len = BUFSIZ;
			//		client = cl;
		};

		void remove();
	};

	struct Client
	{
		SOCKET socket;
		WCHAR addr[16];
		USHORT port;
		std::vector<char> recv_buffer;
		HANDLE lock_mutex;
		std::list<OverlappedDetail*> overlapped_list;
		std::list<Client*>::iterator it;
		WS_Status ws_status;
		LARGE_INTEGER timestamp;

		OverlappedDetail* createOverlapped(Operation operation)
		{
			OverlappedDetail* ol = new OverlappedDetail(this, operation);
			overlapped_list.push_back(ol);
			ol->it = overlapped_list.end();
			--ol->it;
			return ol;
		};

		void removeOverlapped(OverlappedDetail* ol)
		{
			overlapped_list.erase(ol->it);
			delete ol;
		};

		Client(SOCKET s);
		~Client();
	};

	enum CallbackStatus
	{
		cbs_listening,
		cbs_connected,
		cbs_ws_connecting,
		cbs_ws_connected,
		cbs_disconnected,
		cbs_server_stopped,
		cbs_ws_recv,
		cbs_ws_ping,
		cbs_ws_pong,
	};

	struct CallbackMsg
	{
		CallbackStatus status;
		void* data;

		void* owner;
	};

	struct CallbackAddr
	{
		WCHAR* addr;
		USHORT port;
	};

	struct CallbackRecv
	{
		Client* client;
		char* data;
		unsigned long long len;
	};

	enum ServerStatus
	{
		server_status_stopping,
		server_status_stopped,
		server_status_starting,
		server_status_started,
	};

// functions
public:
	bool initServer(USHORT port, void (*callbackFunc)(CallbackMsg* msg) = NULL, void* callbackOwner = NULL);
	void cleanup();
	void log(wchar_t* Format, ...);
	static unsigned int __stdcall workerThread(LPVOID lParam);
	bool recvSocket(OverlappedDetail* overlapped);
	bool sendSocket(OverlappedDetail* overlapped);
	void getElapsedTime(LARGE_INTEGER* timestamp);

	void closeClient(Client* client);
	bool recvMessage(Client* client);
	bool sendMessage(const char* msg, DWORD len, Client* client = NULL);
	bool ping(Client* client = NULL);
	void setCallbackFunc(void (*callbackFunc)(CallbackMsg* msg), void* callbackOwner);

protected:
	bool acceptSocket();
	bool onAccept(OverlappedDetail* overlapped);
	bool onRecv(OverlappedDetail* overlapped, DWORD bytes_recv);
	bool onSend(OverlappedDetail* overlapped);
	bool processRecv(OverlappedDetail* overlapped, char* buffer, DWORD buflen);

	void callbackStatus(CallbackMsg* msg);

protected:

	USHORT listen_port;
	SOCKET listen_socket;
	LPFN_ACCEPTEX acceptEx;
	HANDLE IOCP_handle;
	CRITICAL_SECTION console_cs;
	CRITICAL_SECTION callback_cs;
	int num_threads;
	HANDLE* worker_threads;
	HANDLE shutdown_event;
	LARGE_INTEGER frequency;

#define CLEANUP_FLAG_WSACLEANUP			0x0001
#define CLEANUP_FLAG_LISTENSOCKET		0x0002
#define CLEANUP_FLAG_COMPLETIONPORT		0x0004
#define CLEANUP_FLAG_WORKERTHREAD		0x0008
#define CLEANUP_FLAG_ACCEPTEVENT		0x0010
#define CLEANUP_FLAG_ACCEPTTHREAD		0x0020
	unsigned int cleanup_flag;

	std::list<WebSocket::Client*> clients;

	void (*callback_func)(CallbackMsg* msg);
	void *callback_owner;

public:

	DWORD server_status;
};

extern WebSocket websocket;