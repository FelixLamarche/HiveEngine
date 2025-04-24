#pragma once
#include "GLFW/glfw3.h"
namespace hive::glfw
{
    void* GetNativeDisplay(GLFWwindow *window);
    void* GetNativeWindow(GLFWwindow *window);
}