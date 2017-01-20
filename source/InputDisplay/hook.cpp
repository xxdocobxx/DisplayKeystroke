#include "stdafx.h"
#include "hook.h"


Hook hook;

Hook::Hook()
{
	hh_keyboard = NULL;
	hh_mouse = NULL;
	memset(keymap, 0, sizeof(keymap));
}

Hook::~Hook()
{
}

void Hook::init(HINSTANCE hinstance, void(*onKeyFunc)(DWORD vkCode, bool toggle, void* obj), void* pass_obj)
{
	hinst = hinstance;
	onKey = onKeyFunc;
	pass_object = pass_obj;
}

void Hook::start()
{
	hh_keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinst, 0);
	hh_mouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hinst, 0);
}

void Hook::stop()
{
	UnhookWindowsHookEx(hh_keyboard);
	UnhookWindowsHookEx(hh_mouse);
	hh_keyboard = NULL;
	hh_mouse = NULL;
}

LRESULT Hook::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		PKBDLLHOOKSTRUCT data = (PKBDLLHOOKSTRUCT)lParam;
		bool toggle = (data->flags & LLKHF_UP) ? false : true;
		DWORD key = (data->flags & LLKHF_EXTENDED) ? data->vkCode | 0x100 : data->vkCode;
		
		if(hook.keymap[key] != toggle)
		{
			hook.keymap[key] = toggle;
			hook.onKey(key, toggle, hook.pass_object);
		}
	}

	return CallNextHookEx(hook.hh_keyboard, nCode, wParam, lParam);
}

LRESULT Hook::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		PMSLLHOOKSTRUCT data = (PMSLLHOOKSTRUCT)lParam;
		char btn;
		bool toggle;

		switch(wParam)
		{
		case WM_MOUSEMOVE:
			goto no_toggle;
		case WM_LBUTTONDOWN:
			btn = 0x1;
			toggle = true;
			break;
		case WM_LBUTTONUP:
			btn = 0x1;
			toggle = false;
			break;
		case WM_RBUTTONDOWN:
			btn = 0x2;
			toggle = true;
			break;
		case WM_RBUTTONUP:
			btn = 0x2;
			toggle = false;
			break;
		case WM_MBUTTONDOWN:
			btn = 0x4;
			toggle = true;
			break;
		case WM_MBUTTONUP:
			btn = 0x4;
			toggle = false;
			break;
		case WM_XBUTTONDOWN:
			btn = 0x4 + (char)(data->mouseData >> 16);
			toggle = true;
			break;
		case WM_XBUTTONUP:
			btn = 0x4 + (char)(data->mouseData >> 16);
			toggle = false;
			break;
		case WM_MOUSEWHEEL:
			btn = ((short)(data->mouseData >> 16) > 0) ? 0xa : 0xb;
			toggle = true;
			break;
		case WM_MOUSEHWHEEL:
			btn = ((short)(data->mouseData >> 16) > 0) ? 0xe : 0xf;
			toggle = true;
			break;
		default:
			goto no_toggle;
		}

		hook.onKey(btn, toggle, hook.pass_object);
	}

no_toggle:

	return CallNextHookEx(hook.hh_mouse, nCode, wParam, lParam);
}
