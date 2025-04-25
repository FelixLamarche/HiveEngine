#pragma once
#include <Graphic/GFX.h>
#include "VkBootstrap.h"
namespace hive::gfx
{
    struct DeviceHandle
    {
        vkb::Instance m_vkb_instance;
        vkb::PhysicalDevice m_vkb_phys_device;
        vkb::Device m_vkb_device;
        VkSurfaceKHR m_vk_surface = VK_NULL_HANDLE;
    };
}

namespace hive::vk
{
    gfx::DeviceHandle* vulkan_device_create(const gfx::DeviceDesc &desc);
    void vulkan_device_destroy(gfx::DeviceHandle *device);
    void vulkan_device_wait_idle(const gfx::DeviceHandle *device);
}

