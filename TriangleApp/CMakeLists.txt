cmake_minimum_required (VERSION 3.11)

set(PROJECT_NAME TriangleApp)

add_executable(${PROJECT_NAME} WIN32 "main.cpp" "TriangleApp.h" "TriangleApp.cpp" "ShapeGenerator.h" "ShapeGenerator.cpp")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_definitions(${PROJECT_NAME} PRIVATE "UNICODE" "_UNICODE")
target_link_libraries(${PROJECT_NAME} PRIVATE "d3d11.lib" "d3d10.lib" "dxgi.lib" "d3dcompiler.lib" "d2d1.lib" "dwrite.lib" "windowscodecs.lib")