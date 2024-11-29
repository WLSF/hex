#include <cstdio>
#include <Windows.h>

#include "CHWindow.h"
#include "CHGraphics.h"

int main() {
    HINSTANCE hInst = GetModuleHandle(nullptr);

    CHWindow* wnd = new CHWindow(hInst);
    CHGraphics* graphics = new CHGraphics();

    MSG msg = { 0 };

    if (int err = wnd->Init()) {
        printf("Window init went bad, code: %d - %d\n", err, GetLastError());
        goto shutdown;
    }

    if (int err = graphics->Init(wnd->get())) {
        printf("Graphics init went bad, code: %d - %d\n", err, GetLastError());
        goto shutdown;
    }

    wnd->Draw();

    graphics->InitLighting();
    graphics->InitTriangle();
    graphics->InitFloor();

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, wnd->get(), 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            graphics->Render();
        }
    }

shutdown:
    delete graphics;
    delete wnd;
    return static_cast<int>(msg.wParam);
}