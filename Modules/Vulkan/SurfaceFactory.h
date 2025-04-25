#pragma once
#include "VkBootstrap.h"
namespace hive
{
    class Display;
}
namespace hive::vk
{
    void vulkan_create_surface(Display *display, VkInstance instance, VkSurfaceKHR *surface);
}
