#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3d10.h>

IDXGISwapChain* swapchain;
ID3D11Device* dev;
ID3D11DeviceContext* devcon;
ID3D11RenderTargetView* backBuffer;

void InitD3D(HWND hWnd);
void CleanD3D();
void RenderFrame();

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hWnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"BaseWindowClass";

    RegisterClassEx(&wc);

    RECT wr = { 0, 0, 500, 400 };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
                          L"BaseWindowClass",    // name of the window class
                          L"Triangle app",   // title of the window
                          WS_OVERLAPPEDWINDOW,    // window style
                          300,    // x-position of the window
                          300,    // y-position of the window
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    ShowWindow(hWnd, nCmdShow);

    InitD3D(hWnd);

    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                CleanD3D();
                break;
            }
        }
        else {
            RenderFrame();
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


void InitD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC scd;

    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 4;
    scd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);

    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
    pBackBuffer->Release();

    devcon->OMSetRenderTargets(1, &backBuffer, NULL);

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = 800;
    viewport.Height = 600;

    devcon->RSSetViewports(1, &viewport);
}

void CleanD3D() {
    swapchain->Release();
    dev->Release(); 
    backBuffer->Release();
    devcon->Release();
}

void RenderFrame() {
    float color[4] = { 37.0f / 256.0f, 133.0f / 256.0f , 75.0f / 256.0f , 1.0f };
    devcon->ClearRenderTargetView(backBuffer, color);

    swapchain->Present(0, 0);
}
