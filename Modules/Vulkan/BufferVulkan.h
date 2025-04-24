#pragma once
#include "Graphic/GFX.h"
#include "vulkan/vulkan.h"
namespace hive::gfx
{
    struct BufferHandle
    {
        VkBuffer buffer;
        VkDeviceMemory memory;

        VkBufferCreateInfo buffer_info;
    };
}

#include "vulkan/vulkan.h"
namespace hive::vk
{
    void createBuffer(const gfx::DeviceHandle* device, VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    hive::gfx::BufferHandle* vulkan_buffer_create(const hive::gfx::DeviceHandle* device, const hive::gfx::BufferDesc& desc);

    void vulkan_buffer_destroy(const hive::gfx::DeviceHandle* device, hive::gfx::BufferHandle* buffer);

    void vulkan_buffer_update(const hive::gfx::DeviceHandle* device, hive::gfx::CommandPoolHandle * command_pool, hive::gfx::BufferHandle* buffer,
                              const void* data, size_t size);

}
