#include "TriangleApp.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE PrevInstance, LPSTR cmdLine, int cmdShow) {
    TriangleApp::Create(instance, cmdLine, cmdShow);
    TriangleApp* app = TriangleApp::GetInstance();

    app->RunAppLoop();
}