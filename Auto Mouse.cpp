
#include "pch.h"
#include <Windows.h>
#include <time.h>
#include <random>
#include <string>

bool Enabled = false;
HDC Program_dc;

struct Button {
	HWND button;
	Button() {};
	void Settings(LPCWSTR text, HWND* window, HINSTANCE* instance, RECT rect) {
		button = CreateWindow(L"BUTTON", text, WS_CHILD | WS_VISIBLE, rect.left, rect.top, rect.right, rect.bottom, *window, NULL, NULL, instance, NULL);
	};

} Go_Button, Stop_Button;

LRESULT CALLBACK windows_Events(HWND h, UINT msg, WPARAM wP, LPARAM IP) { // Events
	switch (msg) {
		case WM_CLOSE: {
			PostQuitMessage(NULL);
			return 0;
		}
		case WM_COMMAND: {
			if (reinterpret_cast<HWND>(IP) == Go_Button.button) { Enabled = true; return 0; }			
			if (reinterpret_cast<HWND>(IP) == Stop_Button.button) { Enabled = false; return 0; }
		}
	}
	return DefWindowProc(h, msg, wP, IP);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR IpCmdLine, _In_ int nCmdShow) { // main
	srand(time(NULL));
	HINSTANCE instance = GetModuleHandle(NULL);
	              
	WNDCLASS Program_window;
	Program_window.style = NULL;
	Program_window.lpfnWndProc = &windows_Events;
	Program_window.cbClsExtra = Program_window.cbWndExtra = NULL;
	Program_window.hInstance = instance;
	Program_window.hIcon = NULL;
	Program_window.hCursor = NULL;
	Program_window.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));
	Program_window.lpszMenuName = NULL;
	Program_window.lpszClassName = L"window";
	RegisterClass(&Program_window);

	HWND window = CreateWindow(L"window", L"auto mouse", WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX, 100, 100, 500, 300, NULL, NULL, instance, NULL);
	Go_Button.Settings(L"Go", &window, &instance, []()->RECT { RECT rect = { 100, 150, 100, 50 };  return rect; }());
	Stop_Button.Settings(L"Stop", &window, &instance, []()->RECT { RECT rect = { 300, 150, 100, 50 };  return rect; }());
	Program_dc = GetDC(window);

	MSG message;
	message.message = UINT(~WM_QUIT);
	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		} else {
			TextOut(Program_dc, 120, 80, L"AUTO MOUSE ( Pause to Control Key )", lstrlen(L"AUTO MOUSE ( Pause to Control Key )"));
			if (!(GetAsyncKeyState(VK_LCONTROL) & 0x8000) && Enabled) { // Pause to Control Key
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(1);
			}
		}
	}
	ReleaseDC(window, Program_dc);
	DestroyWindow(window);
	UnregisterClass(TEXT("window"), instance);
	return 0;
}
