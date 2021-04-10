#pragma once

#include <cstdint>
#include <memory>
#include <windows.h>
#include <d2d1.h>
#include "ShapeGenerator.h"

template <class T> void SafeRelease(T** ppT) {
    if (*ppT) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

struct Triangle {
    D2D1_POINT_2F p0, p1, p2;
};

class TriangleApp {
public:
    static void Create(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
    static TriangleApp* GetInstance();

    ~TriangleApp();
    void RunAppLoop();
    void SetMousePos(int x, int y);
    void SetAppStop(bool shouldStop);
    void SetTriangleCoords(Triangle triangle);
    ID2D1Factory* GetFactory();
private:
    static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

    TriangleApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
    void InitializeD2D();
    void OnDraw();
    void OnResize(uint32_t width, uint32_t height);
private:
    static TriangleApp* instance;
    static bool isPresent;

    HWND windowHandle;
    uint32_t width, height;
    int xMousePos, yMousePos;
    bool shouldStop;
    ShapeGenerator shapeGenerator;
    Triangle triangle;
// Direct2D stuff
private:
    ID2D1Factory* d2dFactory;
    ID2D1HwndRenderTarget* renderTarget;
    ID2D1SolidColorBrush* brush;
    ID2D1LinearGradientBrush* linearGradientBrush;
};