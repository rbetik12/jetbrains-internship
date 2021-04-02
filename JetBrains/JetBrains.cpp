#include <memory>
#include <vector>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include "Utils/Init.h"
#include "UI/UI.h"
#include "Utils/Search/Search.h"

#define INPUT_BUFFER_SIZE 128

bool IsInputChanged(char* currentBuf, char* prevBuff) {
    return std::memcmp(currentBuf, prevBuff, INPUT_BUFFER_SIZE);
}

int main() {
    GLFWwindow* window = InitGLFW();

    if (!window) {
        return -1;
    }

    InitImGui(window);

    char* inputBuffer = new char[INPUT_BUFFER_SIZE];
    char* inputBufferCopy = new char[INPUT_BUFFER_SIZE];
    std::vector<std::string> searchResults;
    std::memset(inputBuffer, 0, INPUT_BUFFER_SIZE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        std::memcpy(inputBufferCopy, inputBuffer, INPUT_BUFFER_SIZE);

        UIBegin();
        {
            ImGui::Begin("My First Tool");
            ImGui::InputText("Write here word you want to find", inputBuffer, INPUT_BUFFER_SIZE);

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Search results");
            ImGui::BeginChild("Scrolling");
            for (auto res : searchResults) {
                ImGui::Text("%s", res.c_str());
            }
            ImGui::EndChild();
            ImGui::End();
        }
        UIEnd(window);

        if (IsInputChanged(inputBuffer, inputBufferCopy)) {
            Search(searchResults, std::string(inputBuffer));
        }

        glfwSwapBuffers(window);
    }

    delete[] inputBuffer;
    delete[] inputBufferCopy;
    Destroy();
    return 0;
}