cmake_minimum_required (VERSION 3.8)

include_directories("../lib/glfw/include")
include_directories("../lib/imgui")
include_directories("../lib/glad/include")


add_executable(JetBrains "JetBrains.cpp" "UI/ImGuiBuild.cpp" "Utils/Init.h" "Utils/Init.cpp" "UI/UI.h" "UI/UI.cpp" "Utils/Search/Search.h" "Utils/Search/Search.cpp" "Utils/Threading/ThreadPool.h" "Utils/Threading/ThreadPool.cpp" "Utils/Threading/ThreadTask.h" "Utils/Threading/TaskScheduler.h" "Utils/Threading/TaskScheduler.cpp")
target_link_libraries(JetBrains glad glfw imgui)

