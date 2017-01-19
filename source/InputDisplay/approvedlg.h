// approvedlg.h : Declaration of the Capprovedlg

#pragma once

#include "resource.h"       // main symbols

#include <atlhost.h>

#include "websocket.h"

using namespace ATL;

// Capprovedlg

class Capprovedlg : 
	public CAxDialogImpl<Capprovedlg>
{
public:

	struct ApproveDlgData
	{
		Capprovedlg* parent_ref;
		WebSocket::Client* client;
		void* parent;
	};

protected:

	ApproveDlgData** data;

public:

	Capprovedlg() :
		data(NULL)
	{
	}

	~Capprovedlg()
	{
	}

	enum { IDD = IDD_APPROVEDLG };

BEGIN_MSG_MAP(Capprovedlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<Capprovedlg>)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};


