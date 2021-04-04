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

bool IsInputChanged(char* currentBuf, char* prevBuff) {
    return std::memcmp(currentBuf, prevBuff, INPUT_BUFFER_SIZE);
}

int main() {
    FILE* dictionary = fopen("big-dic.txt", "r");
    if (!dictionary) {
        return -1;
    }
    GLFWwindow* window = InitGLFW();

    if (!window) {
        return -1;
    }

    InitImGui(window);

    char* inputBuffer = new char[INPUT_BUFFER_SIZE];
    char* inputBufferCopy = new char[INPUT_BUFFER_SIZE];
    std::vector<std::string> searchResults;
    std::memset(inputBuffer, 0, INPUT_BUFFER_SIZE);
    TaskScheduler scheduler;
    SchedulerTask task;
    int pageNum = 0;
    const int pageSize = 32;

    //TO-DO
    /*
    * 1. Fix UI fleckreing during search
    * 2. Remove thread pool
    * 3. Reset search thread correctly
    */

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        std::memcpy(inputBufferCopy, inputBuffer, INPUT_BUFFER_SIZE);
        UIBegin();
        {
            ImGui::Begin("My First Tool");
            ImGui::InputText("Write here word you want to find", inputBuffer, INPUT_BUFFER_SIZE);

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Search results");
            if (searchResults.size() > pageSize) {
                if (ImGui::Button("Next")) {
                    if (pageNum + pageSize < searchResults.size()) {
                        pageNum += pageSize;
                    }
                    std::cout << "Page num is " << pageNum << std::endl;
                }
                if (ImGui::Button("Prev")) {
                    if (pageNum - pageSize >= 0) {
                        pageNum -= pageSize;
                    }
                    std::cout << "Page num is " << pageNum << std::endl;
                }
                ImGui::Text("Pages: %d/%d", pageNum, searchResults.size() / pageSize);
            }

            ImGui::BeginChild("Scrolling");
            if (searchResults.size() <= pageSize) {
                for (size_t i = 0; i < searchResults.size(); i++) {
                    ImGui::TextUnformatted(searchResults[i].c_str());
                }
            }
            else {
                for (size_t i = pageNum; i < pageSize + pageNum; i++) {
                    if (i >= searchResults.size()) break;
                    ImGui::TextUnformatted(searchResults[i].c_str());
                }
            }
            ImGui::EndChild();

            ImGui::End();
        }
        UIEnd(window);

        if (IsInputChanged(inputBuffer, inputBufferCopy)) {
            searchResults.clear();
            task.dictionary = dictionary;
            task.results = &searchResults;
            task.stringToSearch = std::string(inputBuffer);
            pageNum = 0;
            if (!task.stringToSearch.empty()) {
                scheduler.RequestSearch(task);
            }
        }

        glfwSwapBuffers(window);
    }

    delete[] inputBuffer;
    delete[] inputBufferCopy;
    Destroy();
    return 0;
}