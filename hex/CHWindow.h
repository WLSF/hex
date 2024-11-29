#pragma once
#include <Windows.h>
#include <string>

class CHWindow {
private:
	WNDCLASS wc = { 0 };
	HWND hWnd;

	std::wstring lpClassName = L"HextechWindowClass",
				 lpWindowName = L"Hextech";

	int width = 800,
		height = 600;

	HINSTANCE hInstRef;

public:
	CHWindow(HINSTANCE hInst) : hWnd(nullptr), hInstRef(hInst) {}
	~CHWindow();

	int Init();
	void Draw();

	HWND get() {
		return hWnd;
	}
};