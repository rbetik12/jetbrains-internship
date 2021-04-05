#pragma once
#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

void Destroy();
void InitImGui(GLFWwindow* window);
GLFWwindow* InitGLFW();