#pragma once
#include "Graphic/GFX.h"

#include "vulkan/vulkan.h"
namespace hive::gfx
{
    struct CommandPoolHandle
    {
        VkCommandPool command_pool;

    };

    struct CommandBufferHandle
    {
        std::vector<VkCommandBuffer> command_buffers;
    };
}

namespace hive::vk
{
    gfx::CommandPoolHandle* vulkan_command_pool_create(const hive::gfx::DeviceHandle *device, const hive::gfx::CommandPoolDesc &desc);
    void vulkan_command_pool_destroy(const hive::gfx::DeviceHandle *device, hive::gfx::CommandPoolHandle *command_pool);

    gfx::CommandBufferHandle* vulkan_command_buffer_allocate(const hive::gfx::DeviceHandle *device, const hive::gfx::CommandBufferDesc &desc);
    void vulkan_command_buffer_free(const hive::gfx::DeviceHandle *device, gfx::CommandPoolHandle* command_pool, gfx::CommandBufferHandle *command_buffer);


    VkCommandBuffer beginSingleTimeCommands(const gfx::DeviceHandle *device, gfx::CommandPoolHandle *command_pool);
    void endSingleTimeCommands(const gfx::DeviceHandle* device, gfx::CommandPoolHandle *command_pool, VkCommandBuffer commandBuffer);
}
