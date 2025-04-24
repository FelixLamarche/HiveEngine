#pragma once
#include "vulkan/vulkan.h"
namespace hive::gfx
{
    struct RenderpassHandle
    {
        VkRenderPass render_pass;
    };

    struct DeviceHandle;
    struct RenderpassDesc;
}

namespace hive::vk
{
    gfx::RenderpassHandle* vulkan_renderpass_create(const gfx::DeviceHandle *device, const gfx::RenderpassDesc & desc);
    void vulkan_renderpass_destroy(const gfx::DeviceHandle* device, gfx::RenderpassHandle *render_pass);
}
