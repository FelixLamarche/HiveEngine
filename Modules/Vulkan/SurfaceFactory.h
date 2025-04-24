#pragma once

#include "VkBootstrap.h"
namespace hive
{
    class Display;
}
namespace hive::vk
{
    void CreateSurface(Display *display, VkInstance instance, VkSurfaceKHR *surface);
}
