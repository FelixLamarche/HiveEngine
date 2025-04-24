#include "Core/Logging/LogManager.h"
#include "Graphic/GFX.h"
#include "RenderpassVulkan.h"
#include "GraphicDeviceVulkan.h"
#include "SwapchainVulkan.h"

hive::gfx::RenderpassHandle * hive::vk::vulkan_renderpass_create(const gfx::DeviceHandle *device, const gfx::RenderpassDesc & desc)
{
    HIVE_LOG_DEBUG("Creating renderpass handle");
    //Color texture
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = desc.swapchain->swapchain.image_format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    //Depth texture
    VkAttachmentDescription depthAttachment{};
    // depthAttachment.format = find_depth_format(device);
    depthAttachment.format = VK_FORMAT_D32_SFLOAT; //TODO find_depth_format
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachment_descriptions = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<unsigned int>(attachment_descriptions.size());
    renderPassInfo.pAttachments = attachment_descriptions.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkRenderPass render_pass;
    if (vkCreateRenderPass(device->m_vkb_device, &renderPassInfo, nullptr, &render_pass) !=
        VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan render pass!");
        return nullptr;
    }


    const auto handle = static_cast<gfx::RenderpassHandle *>(HV_NEW(sizeof(gfx::RenderpassHandle), HIVE_ALLOC_CAT_ENGINE));
    handle->render_pass = render_pass;

    return handle;
}

void hive::vk::vulkan_renderpass_destroy(const gfx::DeviceHandle *device, gfx::RenderpassHandle *render_pass)
{
    HIVE_LOG_DEBUG("Destroying renderpass handle");
    vkDestroyRenderPass(device->m_vkb_device, render_pass->render_pass, nullptr);
    HV_FREE(render_pass);
}
