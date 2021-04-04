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
#include "Utils/Threading/TaskScheduler.h"
#include <iostream>
#include <unordered_set>

#define INPUT_BUFFER_SIZE 128

void Search(std::vector<std::string>& searchResults, TaskScheduler& scheduler, FILE* dictionary, int pageNum, const char* inputBuffer) {
    SchedulerTask task;
    searchResults.clear();
    scheduler.Clear();
    task.dictionary = dictionary;
    task.results = &searchResults;
    task.stringToSearch = std::string(inputBuffer);
    pageNum = 0;
    if (!task.stringToSearch.empty()) {
        scheduler.RequestSearch(task);
    }
}

bool IsInputChanged(char* currentBuf, char* prevBuff) {
    return std::memcmp(currentBuf, prevBuff, INPUT_BUFFER_SIZE);
}

bool isEnterPressed = false;

int main() {
    FILE* dictionary = fopen("big-dic.txt", "r");
    if (!dictionary) {
        return -1;
    }
    GLFWwindow* window = InitGLFW();

    if (!window) {
        return -1;
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
            isEnterPressed = true;
        }
        if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) {
            isEnterPressed = false;
        }
    });

    InitImGui(window);

    char* inputBuffer = new char[INPUT_BUFFER_SIZE];
    char* inputBufferCopy = new char[INPUT_BUFFER_SIZE];
    std::vector<std::string> searchResults;
    std::memset(inputBuffer, 0, INPUT_BUFFER_SIZE);
    TaskScheduler scheduler;
    int pageNum = 0;
    const int pageSize = 32;

    //TO-DO
    /*
    * 1. Fix UI fleckreing during search
    */

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        UIBegin();
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            int displayW, displayH;
            glfwGetFramebufferSize(window, &displayW, &displayH);
            glViewport(0, 0, displayW, displayH);
            ImGui::SetNextWindowSize(ImVec2(displayW, displayH));
            ImGui::Begin("Searcher", nullptr, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
            ImGui::InputText("Write here word you want to find", inputBuffer, INPUT_BUFFER_SIZE);
            if (ImGui::Button("Search")) {
                Search(searchResults, scheduler, dictionary, pageNum, inputBuffer);
            }

            if (!scheduler.IsSearchFinished()) {
                ImGui::TextColored(ImVec4(1, 0, 1, 1), "Search in progress");
            }

            if (searchResults.size() > pageSize) {
                if (ImGui::Button("Prev")) {
                    if (pageNum * pageSize - pageSize >= 0) {
                        pageNum -= 1;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Next")) {
                    if (pageNum * pageSize + pageSize < searchResults.size()) {
                        pageNum += 1;
                    }
                }                
                ImGui::Text("Page %d/%d", pageNum, searchResults.size() / pageSize);
            }

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Search results");
            ImGui::BeginChild("Scrolling");
            if (searchResults.size() <= pageSize) {
                for (size_t i = 0; i < searchResults.size(); i++) {
                    ImGui::TextUnformatted(searchResults[i].c_str());
                }
            }
            else {
                for (size_t i = pageNum * pageSize; i < pageSize + pageNum * pageSize; i++) {
                    if (i >= searchResults.size()) break;
                    ImGui::TextUnformatted(searchResults[i].c_str());
                }
            }
            ImGui::EndChild();

            ImGui::End();
        }
        UIEnd(window);

        glfwSwapBuffers(window);
    }

    delete[] inputBuffer;
    delete[] inputBufferCopy;
    Destroy();
    return 0;
}