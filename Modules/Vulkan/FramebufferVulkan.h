#pragma once
#include "vulkan/vulkan.h"

namespace hive::gfx
{
    struct FramebufferHandle
    {
        std::vector<VkFramebuffer> framebuffers;
    };

    struct DeviceHandle;
    struct FramebufferDesc;
}

namespace hive::vk
{

    gfx::FramebufferHandle* vulkan_framebuffer_create(const gfx::DeviceHandle *device, const gfx::FramebufferDesc &desc);
    void vulkan_framebuffer_destroy(const gfx::DeviceHandle *device, gfx::FramebufferHandle* framebuffer);
}
