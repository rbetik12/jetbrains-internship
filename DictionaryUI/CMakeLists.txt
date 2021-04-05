cmake_minimum_required (VERSION 3.11)

set(PROJECT_NAME DictionaryUI)

include_directories("../lib/glfw/include")
include_directories("../lib/imgui")
include_directories("../lib/glad/include")

add_executable(${PROJECT_NAME} "JetBrains.cpp" "UI/ImGuiBuild.cpp" "Utils/Init.h" "Utils/Init.cpp" "UI/UI.h" "UI/UI.cpp" "Utils/Search/Search.h" "Utils/Search/Search.cpp"   "Utils/Threading/SchedulerTask.h" "Utils/Threading/TaskScheduler.h" "Utils/Threading/TaskScheduler.cpp")
target_link_libraries(${PROJECT_NAME} glad glfw imgui)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

