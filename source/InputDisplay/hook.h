#pragma once
#include <windows.h>

class Hook
{
public:
	Hook();
	~Hook();

	typedef void(*ONKEY)(DWORD vkCode, bool toggle, void* pass_obj);
	typedef void(*ONMOUSEMOVE)(POINT point, void* pass_obj);

	void init(HINSTANCE hinstance, ONKEY onKeyFunc, ONMOUSEMOVE onMouseMoveFunc, void* pass_obj);
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

	ONKEY onKey;
	ONMOUSEMOVE onMouseMove;
};

extern Hook hook;