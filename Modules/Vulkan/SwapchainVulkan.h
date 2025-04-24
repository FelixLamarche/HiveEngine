#pragma once
#include "Graphic/GFX.h"
#include "VkBootstrap.h"


namespace hive::gfx
{
    struct SwapchainHandle
    {
        vkb::Swapchain swapchain;
        static constexpr int SWAP_IMAGE_VIEW_SIZE = 4;
        VkImageView image_views[SWAP_IMAGE_VIEW_SIZE];
    };
}
namespace hive::vk
{

    gfx::SwapchainHandle* vulkan_swapchain_create(const gfx::DeviceHandle *device, const gfx::SwapchainDesc &swapchain_desc);
    void vulkan_swapchain_destroy(const gfx::DeviceHandle *device, gfx::SwapchainHandle* swapchain);
    gfx::SwapchainHandle* vulkan_swapchain_resize(const hive::gfx::DeviceHandle* device, hive::gfx::SwapchainHandle* prev_swapchain, uint16 width, uint16 height);
}
