#include "UI.h"
#include <glad/glad.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

void UIBegin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIEnd(GLFWwindow* window) {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}