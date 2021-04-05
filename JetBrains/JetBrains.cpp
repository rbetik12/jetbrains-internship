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

void SetFullscreenUI(GLFWwindow* window) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    int displayW, displayH;
    glfwGetFramebufferSize(window, &displayW, &displayH);
    glViewport(0, 0, displayW, displayH);
    ImGui::SetNextWindowSize(ImVec2(displayW, displayH));
}

void Search(std::vector<std::string>& searchResults, TaskScheduler& scheduler, FILE* dictionary, const char* inputBuffer) {
    SchedulerTask task;
    searchResults.clear();
    scheduler.Clear();
    task.dictionary = dictionary;
    task.results = &searchResults;
    task.stringToSearch = std::string(inputBuffer);
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
    std::vector<std::string> searchResults;
    std::memset(inputBuffer, 0, INPUT_BUFFER_SIZE);
    TaskScheduler scheduler;
    int pageNum = 0;
    const int pageSize = 32;

    // We reserve here so much elements to prevent future allocation in a vector. In such way I fixed UI flickering on first search run.
    // In either way program will consume abount 50 mb of ram, so we're just doing that earlier.
    searchResults.reserve(300000);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        UIBegin();
        {
            SetFullscreenUI(window);
            ImGui::Begin("Searcher", nullptr, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
            ImGui::InputText("Write here word you want to find", inputBuffer, INPUT_BUFFER_SIZE);
            if (ImGui::Button("Search") || isEnterPressed) {
                isEnterPressed = false;
                Search(searchResults, scheduler, dictionary, inputBuffer);
                pageNum = 0;
            }

            if (!scheduler.IsSearchFinished()) {
                ImGui::TextColored(ImVec4(1, 0, 1, 1), "Search in progress");
            }

            // Here we draw "Next" and "Prev" buttons for pagination UI part. We draw them only if there are more search results than page size
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

            // Here we print search results to the screen with pagination
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Search results");
            ImGui::BeginChild("Scrolling");
            if (searchResults.size() <= pageSize) {
                for (size_t i = 0; i < searchResults.size(); i++) {
                    ImGui::TextUnformatted(searchResults[i].c_str());
                }
            }
            else {
                size_t upperBound = pageSize + pageNum * pageSize;
                // To prevent out of bounds for vector
                if (upperBound >= searchResults.size()) {
                    upperBound = searchResults.size() - 1;
                }
                for (size_t i = pageNum * pageSize; i < upperBound; i++) {
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
    Destroy();
    return 0;
}