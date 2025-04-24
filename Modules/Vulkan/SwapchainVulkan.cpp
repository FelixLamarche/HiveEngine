#include "SwapchainVulkan.h"
#include "GraphicDeviceVulkan.h"

hive::gfx::SwapchainHandle* hive::vk::vulkan_swapchain_create(const gfx::DeviceHandle *device,
    const gfx::SwapchainDesc &swapchain_desc)
{
    HIVE_LOG_DEBUG("Creating swapchain handle");
    vkb::SwapchainBuilder builder(device->m_vkb_device);

    auto swap_ret = builder.build();

    if (!swap_ret)
    {
        HIVE_LOG_ERROR("Failed to create vulkan swapchain");
        return nullptr;
    }

    auto swapchain = static_cast<gfx::SwapchainHandle*>(HV_NEW(sizeof(gfx::SwapchainHandle), HIVE_ALLOC_CAT_ENGINE));
    swapchain->swapchain = swap_ret.value();

    auto image_views = swapchain->swapchain.get_image_views().value();
    HIVE_ASSERT(image_views.size() <= gfx::SwapchainHandle::SWAP_IMAGE_VIEW_SIZE, "The array for a swapchain is not big enough");
    for (uint8 i = 0; i < image_views.size(); i++)
    {
        swapchain->image_views[i] = image_views[i];
    }


    return swapchain;
}

void hive::vk::vulkan_swapchain_destroy(const gfx::DeviceHandle *device, gfx::SwapchainHandle *swapchain)
{
    HIVE_LOG_DEBUG("Destroying swapchain handle");
    vkb::destroy_swapchain(swapchain->swapchain);
    for (int i = 0; i < gfx::SwapchainHandle::SWAP_IMAGE_VIEW_SIZE; i++)
    {
        vkDestroyImageView(device->m_vkb_device, swapchain->image_views[i], nullptr);
    }
    HV_FREE(swapchain);
}

hive::gfx::SwapchainHandle* hive::vk::vulkan_swapchain_resize(const hive::gfx::DeviceHandle *device,
    hive::gfx::SwapchainHandle* prev_swapchain, uint16 width, uint16 height)
{
    HIVE_LOG_DEBUG("Resizing swapchain handle");
    vkb::SwapchainBuilder builder(device->m_vkb_device);

    auto swap_ret = builder
            .set_old_swapchain(prev_swapchain->swapchain)
            .build();

    if (!swap_ret)
    {
        HIVE_LOG_ERROR("Failed to create vulkan swapchain");
        return nullptr;
    }

    vulkan_swapchain_destroy(device, prev_swapchain);

    auto swapchain = static_cast<gfx::SwapchainHandle*>(HV_NEW(sizeof(gfx::SwapchainHandle), HIVE_ALLOC_CAT_ENGINE));
    swapchain->swapchain = swap_ret.value();
    return swapchain;

}
