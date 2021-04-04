cmake_minimum_required (VERSION 3.11)

include_directories("../lib/glfw/include")
include_directories("../lib/imgui")
include_directories("../lib/glad/include")

add_executable(JetBrains "JetBrains.cpp" "UI/ImGuiBuild.cpp" "Utils/Init.h" "Utils/Init.cpp" "UI/UI.h" "UI/UI.cpp" "Utils/Search/Search.h" "Utils/Search/Search.cpp"   "Utils/Threading/ThreadTask.h" "Utils/Threading/TaskScheduler.h" "Utils/Threading/TaskScheduler.cpp")
target_link_libraries(JetBrains glad glfw imgui)

set_property(TARGET JetBrains PROPERTY CXX_STANDARD 17)

