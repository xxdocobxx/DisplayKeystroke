// maindlg.cpp : Implementation of Cmaindlg

#include "stdafx.h"
#include "maindlg.h"
#include <Richedit.h>

// Cmaindlg

void Cmaindlg::init()
{
	HINSTANCE hinstance = _AtlBaseModule.GetResourceInstance();
	HICON hIcon = LoadIconW(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	loadConfigFile();

	// richedit
	richedit.Attach(GetDlgItem(IDC_RICHEDIT21));
	CHARFORMAT format;
	format.cbSize = sizeof(CHARFORMAT);
	richedit.SendMessage(EM_GETCHARFORMAT, SCF_ALL, (LPARAM)&format);
	format.dwMask = CFM_COLOR;
	format.dwEffects = 0;
	format.crTextColor = RGB(0x00, 0xff, 0x00);
	richedit.SendMessage(EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);
	richedit.SendMessage(EM_SETBKGNDCOLOR, 0, RGB(0, 0, 0));

	// hook
	hook.init(hinstance, onKey, this);
	hook.start();
}

void Cmaindlg::cleanup()
{
	richedit = NULL;
	host_port_edit = NULL;
	client_ip_edit = NULL;

	if(about_dlg)
	{
		delete about_dlg;
		about_dlg = NULL;
	}

	if(approve_dlg_data)
	{
		delete approve_dlg_data;
		approve_dlg_data = NULL;
	}

	requesting_clients.clear();

	hook.stop();

	websocket.cleanup();
}

LRESULT Cmaindlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	CAxDialogImpl<Cmaindlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	bHandled = TRUE;

	init();

	return 1;
}

void Cmaindlg::OnFinalMessage(HWND hWnd)
{
	CAxDialogImpl::OnFinalMessage(hWnd);

	cleanup();
}

LRESULT Cmaindlg::OnClickedAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if(about_dlg == NULL)
	{
		about_dlg = new Caboutdlg;
		if(about_dlg->Create(m_hWnd, (LPARAM)&about_dlg) == NULL)
		{
			delete about_dlg;
			about_dlg = NULL;
			return 0;
		}
		about_dlg->CenterWindow(m_hWnd);
	}
	about_dlg->ShowWindow(SW_SHOW);
	about_dlg->SetFocus();
		
	return 0;
}

LRESULT Cmaindlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	saveConfigFile();

	EndDialog(0);
	return 0;
}

LRESULT Cmaindlg::OnBnClickedButtonStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CWindow start_btn(hWndCtl);
	int port;

	switch(websocket.server_status)
	{
	case WebSocket::server_status_stopped:
		start_btn.EnableWindow(FALSE);
		host_port_edit.EnableWindow(FALSE);
		host_port_edit.GetWindowText(tmp_str, MAX_TMP_STR);
		port = (*tmp_str != 0) ? _wtoi(tmp_str) : -1;
		if(port < 0 || port > 0xffff)
		{
			addText(_T("Valid port numbers are 0 - 65535. 0 = auto\n"));
			host_port_edit.EnableWindow(TRUE);
		}
		else if(!websocket.initServer(_wtoi(tmp_str), wsCallback, (void*)this))
		{
			addText(_T("Failed to start server.\n"));
			host_port_edit.EnableWindow(TRUE);
		}
		else
			start_btn.SetWindowText(_T("Stop"));
		start_btn.EnableWindow(TRUE);
		break;

	case WebSocket::server_status_started:
		start_btn.EnableWindow(FALSE);
		websocket.cleanup();
		start_btn.SetWindowText(_T("Start"));
		start_btn.EnableWindow(TRUE);
		host_port_edit.EnableWindow(TRUE);
		break;
	}

	return 0;
}

LRESULT Cmaindlg::OnBnClickedCheckClientIP(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	client_ip_edit.EnableWindow(IsDlgButtonChecked(wID) == BST_CHECKED);
	return 0;
}

DLGPROC Cmaindlg::GetDialogProc()
{
	return mainDlgProc;
}

INT_PTR Cmaindlg::mainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Cmaindlg* pThis = reinterpret_cast<Cmaindlg*>(hWnd);

	switch(uMsg)
	{
	case WM_OPEN_APPROVEDLG:
		pThis->openApproveDlg();
		break;
	}

	return CAxDialogImpl<Cmaindlg>::DialogProc(hWnd, uMsg, wParam, lParam);
}

void Cmaindlg::onKey(DWORD vkCode, bool toggle, void* pass_obj)
{
	((Cmaindlg*)pass_obj)->sendKey(vkCode, toggle);
}

void Cmaindlg::sendKey(DWORD vkCode, bool toggle)
{
	if(vkCode > 0x200)
	{
		websocket.log(L"Unexpected vkCode: %d", vkCode);
		return;
	}
	char msg[] = "-000";
	DWORD len = sizeof(msg);
	char toggle_char = (toggle ? '+' : '-');
	sprintf_s(msg, len, "%c%03x", toggle_char, vkCode);

	websocket.sendMessage(msg, len);
}

void Cmaindlg::addText(LPCTSTR text, ...)
{
	if(richedit)
	{
		int lines = (int)richedit.SendMessage(EM_GETLINECOUNT, 0, 0);
		if(lines > 100)
		{
			int second_line = (int)richedit.SendMessage(EM_LINEINDEX, 1, 0);
			richedit.SendMessage(EM_SETSEL, 0, (LPARAM)second_line);
			richedit.SendMessage(EM_REPLACESEL, FALSE, (LPARAM)_T(""));
		}

		va_list args;
		va_start(args, text);

		_vsnwprintf_s(tmp_str, MAX_TMP_STR, text, args);

		va_end(args);

		richedit.SendMessage(EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
		richedit.SendMessage(EM_REPLACESEL, FALSE, (LPARAM)tmp_str);
		richedit.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
	}
}

void Cmaindlg::wsCallback(WebSocket::CallbackMsg * msg)
{
	if(msg->owner)
	{
		Cmaindlg* main_dlg = (Cmaindlg*)msg->owner;
		WebSocket::Client* client;
		WebSocket::CallbackAddr* addr;

		switch(msg->status)
		{
		case WebSocket::cbs_listening:
			addr = (WebSocket::CallbackAddr*)msg->data;
			main_dlg->addText(_T("Server started at %s:%d.\n"), addr->addr, addr->port);
			break;

		case WebSocket::cbs_connected:
			break;

		case WebSocket::cbs_ws_connecting:
			break;

		case WebSocket::cbs_ws_connected:
			client = (WebSocket::Client*)msg->data;
			main_dlg->addText(_T("Client %s:%d request to connect...\n"), client->addr, client->port);

			if(main_dlg->IsDlgButtonChecked(IDC_CHECK_CLIENTIP) == BST_CHECKED)
			{
				main_dlg->GetDlgItemText(IDC_IPADDRESS_CLIENT, main_dlg->tmp_str, MAX_TMP_STR);
				if(_tcscmp(main_dlg->tmp_str, client->addr) == 0)
					main_dlg->acceptConnection(client);
				else
					websocket.closeClient(client);
			}
			else
			{
				main_dlg->requesting_clients.push_back(client);
				main_dlg->SendMessage(WM_OPEN_APPROVEDLG);
			}
			break;

		case WebSocket::cbs_disconnected:
			client = (WebSocket::Client*)msg->data;
			main_dlg->addText(_T("Client %s:%d disconnected.\n"), client->addr, client->port);

			main_dlg->requesting_clients.remove(client);
			if(main_dlg->approve_dlg_data && main_dlg->approve_dlg_data->client == client)
				main_dlg->approve_dlg_data->parent_ref->SendMessage(WM_CLOSE);
			break;

		case WebSocket::cbs_server_stopped:
			main_dlg->addText(_T("Server stopped.\n"));
			break;

		case WebSocket::cbs_ws_pong:
			client = (WebSocket::Client*)msg->data;
			main_dlg->addText(_T("Client %s:%d ping: %.3fms\n"), client->addr, client->port, (double)client->timestamp.QuadPart / 1000.0);
			break;
		}
	}
}

void Cmaindlg::approveConnection(bool accept)
{
	if(approve_dlg_data)
	{
		delete approve_dlg_data;
		approve_dlg_data = NULL;
	}

	if(!requesting_clients.empty())
	{
		WebSocket::Client* client = requesting_clients.front();
		requesting_clients.pop_front();

		if(accept)
			acceptConnection(client);
		else
			websocket.closeClient(client);

		openApproveDlg();
	}
}

void Cmaindlg::openApproveDlg()
{
	if(approve_dlg_data == NULL && !requesting_clients.empty())
	{
		approve_dlg_data = new Capprovedlg::ApproveDlgData;
		Capprovedlg* approvedlg = approve_dlg_data->parent_ref = new Capprovedlg;
		approve_dlg_data->parent = (void*)this;
		approve_dlg_data->client = requesting_clients.front();
		if(approvedlg->Create(m_hWnd, (LPARAM)&approve_dlg_data) == NULL)
		{
			delete approvedlg;
			delete approve_dlg_data;
			approve_dlg_data = NULL;

			for(std::list<WebSocket::Client*>::iterator it = requesting_clients.begin(); it != requesting_clients.end(); ++it)
				websocket.closeClient(*it);
			requesting_clients.clear();
		}
		else
		{
			approvedlg->CenterWindow(m_hWnd);
			approvedlg->ShowWindow(SW_SHOW);
			approvedlg->SetFocus();
			approvedlg->BringWindowToTop();

			FLASHWINFO flashwinfo;
			flashwinfo.cbSize = sizeof(FLASHWINFO);
			flashwinfo.hwnd = approvedlg->m_hWnd;
			flashwinfo.dwFlags = FLASHW_ALL;
			flashwinfo.uCount = 5;
			flashwinfo.dwTimeout = 0;
			FlashWindowEx(&flashwinfo);

			flashwinfo.hwnd = m_hWnd;
			flashwinfo.dwFlags = FLASHW_TRAY;
			FlashWindowEx(&flashwinfo);
		}
	}
}

void Cmaindlg::acceptConnection(WebSocket::Client* client)
{
	addText(_T("Client %s:%d connected.\n"), client->addr, client->port);
	client->ws_status = WebSocket::ws_status_connected;
	char msg[] = "server approved";
	websocket.sendMessage(msg, (DWORD)sizeof(msg), client);
	websocket.ping(client);
}

void Cmaindlg::loadConfigFile()
{
	CAtlFile file;
	char* buf = NULL;
	ULONGLONG ull_len = 0;
	CString str;

	try
	{
		if(file.Create(_T("config.js"), GENERIC_READ, 0, OPEN_EXISTING) != S_OK)
			throw 0;

		if(file.GetSize(ull_len) != S_OK)
			throw 1;

		DWORD len = (DWORD)ull_len;
		buf = new char[len + 1];

		if(file.Read(buf, len) != S_OK)
			throw 2;

		buf[len] = 0;
		str = buf;

		delete[] buf;
		file.Close();
	}
	catch(int err)
	{
		if(err > 0)
			file.Close();
		if(err > 1)
			delete[] buf;
	}

	// edit port
	host_port_edit.Attach(GetDlgItem(IDC_EDIT_PORT));
	host_port_edit.SetWindowText(getValueFromString(str, _T("var host_port ="), _T("7788")));

	// client ip
	client_ip_edit.Attach(GetDlgItem(IDC_IPADDRESS_CLIENT));
	client_ip_edit.SetWindowText(getValueFromString(str, _T("var client_ip ="), _T("127.0.0.1")));
	client_ip_edit.EnableWindow(FALSE);

	// auto approve
	UINT auto_approve = (getValueFromString(str, _T("var auto_approve"), _T("false")).CompareNoCase(_T("true")) == 0) ? BST_CHECKED : BST_UNCHECKED;
	CheckDlgButton(IDC_CHECK_CLIENTIP, auto_approve);
	client_ip_edit.EnableWindow(auto_approve == BST_CHECKED);
}

void Cmaindlg::saveConfigFile()
{
	CAtlFile file;
	char* buf = NULL;
	ULONGLONG ull_len = 0;
	CString str;
	TCHAR filename[] = _T("config.js");

	try
	{
		if(file.Create(filename, GENERIC_READ, 0, OPEN_EXISTING) != S_OK)
			throw 0;

		if(file.GetSize(ull_len) != S_OK)
			throw 1;

		DWORD len = (DWORD)ull_len;
		buf = new char[len + 1];

		if(file.Read(buf, len) != S_OK)
			throw 2;

		buf[len] = 0;
		str = buf;

		file.Close();
		delete[] buf;
	}
	catch(int err)
	{
		if(err > 0)
			file.Close();
		if(err > 1)
			delete[] buf;
	}

	// host port
	CString tmp;
	host_port_edit.GetWindowText(tmp);
	replaceValueFromString(str, _T("var host_port ="), tmp);

	// client ip
	client_ip_edit.GetWindowText(tmp);
	replaceValueFromString(str, _T("var client_ip ="), tmp);

	// auto approve
	replaceValueFromString(str, _T("var auto_approve ="), (IsDlgButtonChecked(IDC_CHECK_CLIENTIP) == BST_CHECKED ? _T("true") : _T("false")));

	// write to file
	if(file.Create(filename, GENERIC_WRITE, 0, CREATE_ALWAYS) != S_OK)
		return;

	CT2A ascii(str);

	DWORD written_bytes;
	if(file.Write((LPCVOID)ascii.m_psz, (DWORD)strlen(ascii.m_psz), &written_bytes) != S_OK)
	{
		str.Format(_T("file.Write failed. %d\n"), GetLastError());
		OutputDebugString(str);
	}

	file.Close();
}

CString Cmaindlg::getValueFromString(CString& string, LPCWSTR find_str, LPCWSTR def_str)
{
	int pos, pos1, pos2;

	pos = string.Find(find_str);
	if(pos != -1)
	{
		pos1 = string.Find(_T("\'"), pos + _tcslen(find_str));
		if(pos1 != -1)
		{
			pos2 = string.Find(_T("\'"), pos1 + 1);
			if(pos2 != -1)
			{
				int len = pos2 - pos1 - 1;
				if(len > 0)
					return string.Mid(pos1 + 1, len);
			}
		}
		else
		{
			pos1 = string.Find(_T("\""), pos + 1);
			if(pos1 != -1)
			{
				pos2 = string.Find(_T("\""), pos1 + 1);
				if(pos2 != -1)
				{
					int len = pos2 - pos1 - 1;
					if(len > 0)
						return string.Mid(pos1 + 1, len);
				}
			}
		}
	}

	return CString(def_str);
}

void Cmaindlg::replaceValueFromString(CString& string, LPCWSTR find_str, LPCWSTR replace_str)
{
	int pos, pos1, pos2;

	CString new_str;
	new_str.Format(_T("%s \'%s\'"), find_str, replace_str);

	pos = string.Find(find_str);
	if(pos != -1)
	{
		pos1 = string.Find(_T("\'"), pos + _tcslen(find_str));
		if(pos1 != -1)
		{
			pos2 = string.Find(_T("\'"), pos1 + 1);
			if(pos2 != -1)
			{
				int len = pos2 - pos + 1;
				if(len > 0)
				{
					string.Replace(string.Mid(pos, len), new_str);
					return;
				}
			}
		}
		else
		{
			pos1 = string.Find(_T("\""), pos + _tcslen(find_str));
			if(pos1 != -1)
			{
				pos2 = string.Find(_T("\""), pos1 + 1);
				if(pos2 != -1)
				{
					int len = pos2 - pos + 1;
					if(len > 0)
					{
						string.Replace(string.Mid(pos, len), new_str);
						return;
					}
				}
			}
		}
	}

	string += new_str + _T(";\r\n");
}
