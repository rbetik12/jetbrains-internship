#pragma once
#include <windows.h>
#include <cstdint>
#include <memory>

class TriangleApp {
public:
    static void Create(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
    static TriangleApp* GetInstance();

public:
    ~TriangleApp();
    void RunAppLoop();
    void SetMousePos(int x, int y);
    void SetAppStop(bool shouldStop);
private:
    TriangleApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
    static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
private:
    HWND windowHandle;
    uint32_t width, height;
    int xMousePos, yMousePos;
    bool shouldStop;
    static TriangleApp* instance;
    static bool isPresent;
};