// approvedlg.cpp : Implementation of Capprovedlg

#include "stdafx.h"
#include "approvedlg.h"
#include "maindlg.h"

// Capprovedlg

LRESULT Capprovedlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	CAxDialogImpl<Capprovedlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	bHandled = TRUE;

	HINSTANCE hinstance = _AtlBaseModule.GetResourceInstance();
	HICON hIcon = LoadIconW(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	data = (ApproveDlgData**)lParam;
	WebSocket::Client* client = (*data)->client;
	TCHAR str[BUFSIZ];
	_stprintf_s(str, BUFSIZ, _T("Connection request from (%s:%d).\n\nApprove this request?"), client->addr, client->port);
	SetDlgItemText(IDC_STATIC_APPROVE_MSG, str);

	return 1;  // Let the system set the focus
}

void Capprovedlg::OnFinalMessage(HWND hWnd)
{
	if(data && *data)
	{
		delete *data;
		*data = NULL;
	}
	delete this;
}

LRESULT Capprovedlg::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	((Cmaindlg*)((*data)->parent))->approveConnection(true);
	DestroyWindow();
	return 0;
}

LRESULT Capprovedlg::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	((Cmaindlg*)((*data)->parent))->approveConnection(false);
	DestroyWindow();
	return 0;
}


LRESULT Capprovedlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	((Cmaindlg*)((*data)->parent))->approveConnection(false);
	DestroyWindow();
	return 0;
}
