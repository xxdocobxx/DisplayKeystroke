// aboutdlg.h : Declaration of the Caboutdlg

#pragma once

#include "resource.h"       // main symbols

#include <atlhost.h>

using namespace ATL;

// Caboutdlg

class Caboutdlg : 
	public CAxDialogImpl<Caboutdlg>
{
protected:

	Caboutdlg** parent_ref;

	void openLink(LPNMHDR pNMHDR)
	{
		PNMLINK pNMLink = (PNMLINK)pNMHDR;
		ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOW);
	}

public:
	Caboutdlg() :
		parent_ref(NULL)
	{
	}

	~Caboutdlg()
	{
	}

	enum { IDD = IDD_ABOUTDLG };

BEGIN_MSG_MAP(Caboutdlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<Caboutdlg>)
	NOTIFY_HANDLER(IDC_SYSLINK_FEEDBACK, NM_CLICK, OnNMClickSyslinkFeedback)
	NOTIFY_HANDLER(IDC_SYSLINK_FEEDBACK, NM_RETURN, OnNMReturnSyslinkFeedback)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<Caboutdlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		bHandled = TRUE;

		// init
		HINSTANCE hinstance = _AtlBaseModule.GetResourceInstance();
		HICON hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
		SetIcon(hIcon, FALSE);

		parent_ref = (Caboutdlg**)lParam;

		return 1;  // Let the system set the focus
	}

	void OnFinalMessage(HWND hWnd)
	{
		// TODO: Add your specialized code here and/or call the base class
		CAxDialogImpl::OnFinalMessage(hWnd);

		if(parent_ref)
			*parent_ref = NULL;
		delete this;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		DestroyWindow();
		return 0;
	}

	LRESULT OnNMClickSyslinkFeedback(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
	{
		openLink(pNMHDR);
		return 0;
	}

	LRESULT OnNMReturnSyslinkFeedback(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
	{
		openLink(pNMHDR);
		return 0;
	}
};


