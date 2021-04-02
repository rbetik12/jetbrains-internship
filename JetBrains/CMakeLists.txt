cmake_minimum_required (VERSION 3.8)

include_directories("../lib/glfw/include")
include_directories("../lib/imgui")
include_directories("../lib/glad/include")


add_executable(JetBrains "JetBrains.cpp" "UI/ImGuiBuild.cpp" "Utils/Init.h" "Utils/Init.cpp" "UI/UI.h" "UI/UI.cpp" "Utils/Search/Search.h" "Utils/Search/Search.cpp")
target_link_libraries(JetBrains glad glfw imgui)

