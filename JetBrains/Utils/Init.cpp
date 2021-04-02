#include "Init.h"
#include <glad/glad.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

void Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void InitImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

GLFWwindow* InitGLFW() {
    GLFWwindow* window;
    if (!glfwInit())
        return nullptr;

    window = glfwCreateWindow(1280, 720, "Dictionary Lookup", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (!status) {
        glfwTerminate();
        return nullptr;
    }

    return window;
}