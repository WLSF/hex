#include "CHWindow.h"

CHWindow::~CHWindow() {
	DestroyWindow(hWnd);
}

LRESULT WINAPI __wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int CHWindow::Init() {
	wc.hInstance = hInstRef;
	wc.lpfnWndProc = __wndProc;
	wc.lpszClassName =lpClassName.c_str();
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&wc)) {
		return 1;
	}

	hWnd = CreateWindow(lpClassName.c_str(), lpWindowName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstRef, nullptr);
	if (!hWnd) {
		return 2;
	}

	return 0;
}

void CHWindow::Draw() {
	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
}