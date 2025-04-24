#include "SyncVulkan.h"

#include "BufferVulkan.h"
#include "CommandVulkan.h"
#include "FramebufferVulkan.h"
#include "GraphicDeviceVulkan.h"
#include "RenderpassVulkan.h"
#include "ShaderVulkan.h"
#include "SwapchainVulkan.h"

hive::gfx::SemaphoreHandle* hive::vk::vulkan_semaphore_create(const gfx::DeviceHandle* device)
{
    VkSemaphoreCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkSemaphore semaphore = VK_NULL_HANDLE;
    if (vkCreateSemaphore(device->m_vkb_device, &create_info, nullptr, &semaphore) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan semaphore");
        return nullptr;
    }

    auto* handle = static_cast<gfx::SemaphoreHandle*>(HV_NEW(sizeof(gfx::SemaphoreHandle), HIVE_ALLOC_CAT_GRAPHIC));
    handle->semaphore = semaphore;

    return handle;
}

void hive::vk::vulkan_semaphore_destroy(const gfx::DeviceHandle* device, gfx::SemaphoreHandle* semaphore)
{
    vkDestroySemaphore(device->m_vkb_device, semaphore->semaphore, nullptr);
    HV_FREE(semaphore);
}

hive::gfx::FenceHandle* hive::vk::vulkan_fence_create(const gfx::DeviceHandle* device, const bool is_signaled)
{
    VkFenceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (is_signaled)
    {
        create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    VkFence fence = VK_NULL_HANDLE;
    if (vkCreateFence(device->m_vkb_device, &create_info, nullptr, &fence) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan fence");
        return nullptr;
    }

    auto *handle = static_cast<gfx::FenceHandle*>(HV_NEW(sizeof(gfx::FenceHandle), HIVE_ALLOC_CAT_GRAPHIC));
    handle->fence = fence;

    return handle;
}

void hive::vk::vulkan_fence_destroy(const gfx::DeviceHandle* device, gfx::FenceHandle* fence)
{
    vkDestroyFence(device->m_vkb_device, fence->fence, nullptr);
    HV_FREE(fence);
}


void hive::vk::vulkan_tmp_draw(int current_frame, const hive::gfx::DeviceHandle *device, hive::gfx::FenceHandle *fence, hive::gfx::SemaphoreHandle *image_availabl_sem, hive::gfx::SemaphoreHandle *render_finished_sem, hive::gfx::SwapchainHandle *swapchain, hive::gfx::CommandBufferHandle *command_buffer_handle, hive::gfx::FramebufferHandle *framebuffer, hive::gfx::RenderpassHandle *render_pass, hive::gfx::ShaderProgramHandle *shader_program_handle, hive::gfx::BufferHandle *vertex_buffer, hive::gfx::BufferHandle *index_buffer, hive::gfx::BindingGroupHandle *binding_group, int indices_count)
{
    vkWaitForFences(device->m_vkb_device, 1, &fence->fence, VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device->m_vkb_device, swapchain->swapchain, UINT64_MAX, image_availabl_sem->semaphore, VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            // recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        // updateUniformBuffer(currentFrame);

        vkResetFences(device->m_vkb_device, 1, &fence->fence);

        vkResetCommandBuffer(command_buffer_handle->command_buffers[current_frame], /*VkCommandBufferResetFlagBits*/ 0);
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(command_buffer_handle->command_buffers[current_frame], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = render_pass->render_pass;
        renderPassInfo.framebuffer = framebuffer->framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->swapchain.extent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(command_buffer_handle->command_buffers[current_frame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(command_buffer_handle->command_buffers[current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, shader_program_handle->pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapchain->swapchain.extent.width;
        viewport.height = (float)swapchain->swapchain.extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(command_buffer_handle->command_buffers[current_frame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapchain->swapchain.extent;
        vkCmdSetScissor(command_buffer_handle->command_buffers[current_frame], 0, 1, &scissor);

        VkBuffer vertexBuffers[] = {vertex_buffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(command_buffer_handle->command_buffers[current_frame], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(command_buffer_handle->command_buffers[current_frame], index_buffer->buffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(command_buffer_handle->command_buffers[current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, shader_program_handle->pipeline_layout, 0, 1,
                                &binding_group->descriptor_sets[current_frame], 0, nullptr);

        vkCmdDrawIndexed(command_buffer_handle->command_buffers[current_frame], indices_count, 1, 0, 0, 0);

        vkCmdEndRenderPass(command_buffer_handle->command_buffers[current_frame]);

        if (vkEndCommandBuffer(command_buffer_handle->command_buffers[current_frame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
        // recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {image_availabl_sem->semaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &command_buffer_handle->command_buffers[current_frame];

        VkSemaphore signalSemaphores[] = {render_finished_sem->semaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(device->m_vkb_device.get_queue(vkb::QueueType::graphics).value(), 1, &submitInfo, fence->fence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapchain->swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(device->m_vkb_device.get_queue(vkb::QueueType::present).value(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            // framebufferResized = false;
            // recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        // currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
