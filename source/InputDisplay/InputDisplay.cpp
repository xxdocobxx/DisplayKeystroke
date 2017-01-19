// InputDisplay.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "InputDisplay_i.h"

//#pragma comment(lib, "comctl32.lib")

#include "maindlg.h"

using namespace ATL;


class CInputDisplayModule : public ATL::CAtlExeModuleT< CInputDisplayModule >
{
public :
	DECLARE_LIBID(LIBID_InputDisplayLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_INPUTDISPLAY, "{B18617A4-79FA-4A32-84BF-27330FA03079}")


	HRESULT PreMessageLoop(int nShowCommand)
	{
		_ATLTRY
		{
			ATLENSURE_SUCCEEDED(__super::PreMessageLoop(nShowCommand));
		}
		_ATLCATCH(Exception)
		{
			return Exception;
		}

		return S_OK;
	};

	void RunMessageLoop()
	{
		Cmaindlg dialog;
		dialog.DoModal();
	};

};

CInputDisplayModule _AtlModule;

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
//	InitCommonControls();
	HINSTANCE m_hLibRichEdit = LoadLibrary(_T("riched20.dll"));

	return _AtlModule.WinMain(nShowCmd);

	FreeLibrary(m_hLibRichEdit);
}