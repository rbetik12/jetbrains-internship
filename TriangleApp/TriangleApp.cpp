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
yMousePos(0),
d2dFactory(nullptr),
renderTarget(nullptr) {
    triangle = { {-1.0f, -1.0f}, {-1.0f, -1.0f}, {-1.0f, -1.0f} };
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
    InitializeD2D();
}

void TriangleApp::InitializeD2D() {
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &d2dFactory
    );

    RECT rc;
    GetClientRect(windowHandle, &rc);

    hr = d2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
        windowHandle,
        D2D1::SizeU(
        rc.right - rc.left,
        rc.bottom - rc.top)
    ),
        &renderTarget
    );

    if (SUCCEEDED(hr)) {
        renderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White, 1.0f),
            &brush
        );
    }

    renderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);


    ID2D1GradientStopCollection* pGradientStops = NULL;

    D2D1_GRADIENT_STOP gradientStops[2];
    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Blue, 1);
    gradientStops[0].position = 0.0f;
    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Orange, 1);
    gradientStops[1].position = 1.0f;

    hr = renderTarget->CreateGradientStopCollection(
        gradientStops,
        2,
        D2D1_GAMMA_2_2,
        D2D1_EXTEND_MODE_CLAMP,
        &pGradientStops
    );

    if (SUCCEEDED(hr)) {
        hr = renderTarget->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(
            D2D1::Point2F(0, 0),
            D2D1::Point2F(150, 150)),
            pGradientStops,
            &linearGradientBrush
        );
    }
}

void TriangleApp::OnDraw() {
    HRESULT hr;
    D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();
    D2D1_RECT_F brushRect = D2D1::RectF(100.0f, 100.0f, renderTargetSize.width - 100.0f, renderTargetSize.height - 100.0f);

    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(37.0f / 256.0f, 133.0f / 256.0f, 75.0f / 256.0f, 1.0f));

    if (triangle.p0.x > 0.0f && triangle.p0.y > 0.0f) {
        auto stroke = ShapeGenerator::CreateStrokeStyle();
        auto triangleGeom = ShapeGenerator::GenerateTriangle(triangle.p0, triangle.p1, triangle.p2);

        renderTarget->DrawGeometry(triangleGeom.Get(), brush, 3.0f, stroke.Get());
        renderTarget->FillGeometry(triangleGeom.Get(), linearGradientBrush);
    }

    hr = renderTarget->EndDraw();
}

void TriangleApp::OnResize(uint32_t width, uint32_t height) {
    if (renderTarget) {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;

        this->width = width;
        this->height = height;

        renderTarget->Resize(size);
        OnDraw();
    }
}

TriangleApp::~TriangleApp() {
    SafeRelease(&renderTarget);
    SafeRelease(&brush);
    SafeRelease(&d2dFactory);
}

void TriangleApp::RunAppLoop() {
    MSG message;

    while (!shouldStop) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        if (DragDetect(windowHandle, { xMousePos, yMousePos })) {
            std::cout << "Draging!" << std::endl;
        }
        OnDraw();
    }
}

void TriangleApp::SetMousePos(int x, int y) {
    xMousePos = x;
    yMousePos = y;
}

void TriangleApp::SetAppStop(bool shouldStop) {
    this->shouldStop = shouldStop;
}

void TriangleApp::SetTriangleCoords(Triangle triangle) {
    this->triangle = triangle;
}

ID2D1Factory* TriangleApp::GetFactory() {
    return d2dFactory;
}

LRESULT TriangleApp::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
    TriangleApp* app = TriangleApp::GetInstance();

    // Check whether application instance was created or not. If not pass current message processing to windows.
    if (!app) return DefWindowProc(windowHandle, message, wParam, lParam);

    switch (message) {
    case WM_LBUTTONDOWN: 
    {
        int xMousePos, yMousePos;
        xMousePos = GET_X_LPARAM(lParam);
        yMousePos = GET_Y_LPARAM(lParam);
        Triangle triangle;

        triangle.p0.x = xMousePos;
        triangle.p0.y = yMousePos;
        triangle.p1.x = xMousePos + 30;
        triangle.p1.y = yMousePos + 50;
        triangle.p2.x = xMousePos + 60;
        triangle.p2.y = yMousePos;
        app->SetTriangleCoords(triangle);
    }
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            ExitProcess(0);
        }
        return 0;
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
    case WM_SIZE:
        uint32_t width = LOWORD(lParam);
        uint32_t height = HIWORD(lParam);

        app->OnResize(width, height);

        return 0;
    }

    return DefWindowProc(windowHandle, message, wParam, lParam);
}

bool TriangleApp::isPresent = false;

