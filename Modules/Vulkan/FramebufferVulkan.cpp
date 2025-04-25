#include "FramebufferVulkan.h"
#include "SwapchainVulkan.h"
#include "TextureVulkan.h"
#include "RenderpassVulkan.h"
#include "GraphicDeviceVulkan.h"

hive::gfx::FramebufferHandle * hive::vk::vulkan_framebuffer_create(const gfx::DeviceHandle *device,
                                                                   const gfx::FramebufferDesc &desc)
{
    std::vector<VkFramebuffer> framebuffers(desc.swapchain->swapchain.image_count);

    for (int i = 0; i < desc.swapchain->swapchain.image_count; i++)
    {
        std::vector<VkImageView> attachments;
        attachments.push_back(desc.swapchain->image_views[i]);
        if (desc.depth_texture != nullptr)
            attachments.push_back(desc.depth_texture->image_view);

        VkFramebufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.renderPass = desc.render_pass->render_pass;
        create_info.attachmentCount = attachments.size();
        create_info.pAttachments = attachments.data();
        create_info.width = desc.swapchain->swapchain.extent.width;
        create_info.height = desc.swapchain->swapchain.extent.height;
        create_info.layers = 1;

        if (vkCreateFramebuffer(device->m_vkb_device, &create_info, nullptr, &framebuffers[i]) != VK_SUCCESS)
        {
            HIVE_LOG_ERROR("Failed to create vulkan framebuffer");
            return nullptr;
        }
    }


    auto handle = static_cast<gfx::FramebufferHandle *>(HV_NEW(sizeof(gfx::FramebufferHandle),  HIVE_ALLOC_CAT_GRAPHIC));
    handle->framebuffers = std::move(framebuffers);

    return handle;
}

void hive::vk::vulkan_framebuffer_destroy(const gfx::DeviceHandle *device, gfx::FramebufferHandle *framebuffer)
{
    for (auto framebuffer : framebuffer->framebuffers)
    {
        vkDestroyFramebuffer(device->m_vkb_device, framebuffer, nullptr);
    }

    HV_FREE(framebuffer);
}
