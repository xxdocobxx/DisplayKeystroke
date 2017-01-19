#pragma once
#include <windows.h>

class Hook
{
public:
	Hook();
	~Hook();

	void init(HINSTANCE hinstance, void (*onKeyFunc)(DWORD vkCode, bool toggle, void* obj), void* pass_obj);
	void start();
	void stop();

	static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

protected:

	HHOOK hh_keyboard;
	HHOOK hh_mouse;
	HINSTANCE hinst;
	void* pass_object;
	bool keymap[512];

	void (*onKey)(DWORD vkCode, bool toggle, void* pass_obj);
};

extern Hook hook;