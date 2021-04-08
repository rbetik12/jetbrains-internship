#include "TriangleApp.h"
#include <windowsx.h>
#include <iostream>
#include <functional>

TriangleApp* TriangleApp::instance = nullptr;

void TriangleApp::Create(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (!isPresent) {
        instance = new TriangleApp(hInstance, lpCmdLine, nCmdShow);
        isPresent = true;

        //Opens console for debug purpose. Mem leak!
        FILE* fp;
        AllocConsole();
        freopen_s(&fp, "CONIN$", "r", stdin);
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
    }
}

TriangleApp* TriangleApp::GetInstance() {
    return instance;
}

TriangleApp::TriangleApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) : width(1280),
                                                                            height(720),
                                                                            shouldStop(false),
                                                                            xMousePos(0),
                                                                            yMousePos(0) {
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"BaseWindowClass";

    RegisterClassEx(&wc);

    windowHandle = CreateWindowEx(NULL,
                                  L"BaseWindowClass",    // name of the window class
                                  L"Triangle app",   // title of the window
                                  WS_OVERLAPPEDWINDOW,    // window style
                                  0,    // x-position of the window
                                  0,    // y-position of the window
                                  width,
                                  height,
                                  NULL,    // we have no parent window, NULL
                                  NULL,    // we aren't using menus, NULL
                                  hInstance,    // application handle
                                  NULL);    // used with multiple windows, NULL

    ShowWindow(windowHandle, nCmdShow);
}

TriangleApp::~TriangleApp() {
    delete instance;
}

void TriangleApp::RunAppLoop() {
    MSG message;

    while (!shouldStop) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        std::cout << "Mouse X: " << xMousePos << " Y: " << yMousePos << std::endl;
    }
}

void TriangleApp::SetMousePos(int x, int y) {
    xMousePos = x;
    yMousePos = y;
}

void TriangleApp::SetAppStop(bool shouldStop) {
    this->shouldStop = shouldStop;
}

LRESULT TriangleApp::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
    TriangleApp* app = TriangleApp::GetInstance();

    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        app->SetAppStop(true);
        return 0;
    case WM_MOUSEMOVE:
        int xMousePos, yMousePos;
        xMousePos = GET_X_LPARAM(lParam);
        yMousePos = GET_Y_LPARAM(lParam);

        app->SetMousePos(xMousePos, yMousePos);

        return 0;
    }

    return DefWindowProc(windowHandle, message, wParam, lParam);
}

bool TriangleApp::isPresent = false;

