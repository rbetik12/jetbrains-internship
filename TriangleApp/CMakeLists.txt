cmake_minimum_required (VERSION 3.11)

set(PROJECT_NAME TriangleApp)

add_executable(${PROJECT_NAME} WIN32 "main.cpp")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)