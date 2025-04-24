#pragma once
#include "Graphic/GFX.h"
#include "vulkan/vulkan.h"

namespace hive::gfx
{
    struct SemaphoreHandle
    {
        VkSemaphore semaphore;
    };

    struct FenceHandle
    {
        VkFence fence;
    };
}

namespace hive::vk
{
    gfx::SemaphoreHandle* vulkan_semaphore_create(const gfx::DeviceHandle *device);
    void vulkan_semaphore_destroy(const gfx::DeviceHandle *device, gfx::SemaphoreHandle *semaphore);

    gfx::FenceHandle* vulkan_fence_create(const gfx::DeviceHandle *device, bool is_signaled);
    void vulkan_fence_destroy(const gfx::DeviceHandle *device, gfx::FenceHandle *fence);


    void vulkan_tmp_draw(int current_frame, const hive::gfx::DeviceHandle *device, hive::gfx::FenceHandle *fence, hive::gfx::SemaphoreHandle *image_availabl_sem, hive::gfx::SemaphoreHandle *render_finished_sem, hive::gfx::SwapchainHandle *swapchain, hive::gfx::CommandBufferHandle *command_buffer_handle, hive::gfx::FramebufferHandle *framebuffer, hive::gfx::RenderpassHandle *render_pass, hive::gfx::ShaderProgramHandle *shader_program_handle, hive::gfx::BufferHandle *vertex_buffer, hive::gfx::BufferHandle *index_buffer, hive::gfx::BindingGroupHandle *binding_group, int indices_count);
}