#include "CommandVulkan.h"
#include "Graphic/GFX.h"
#include "GraphicDeviceVulkan.h"

hive::gfx::CommandPoolHandle * hive::vk::vulkan_command_pool_create(const hive::gfx::DeviceHandle *device,
                                                                    const hive::gfx::CommandPoolDesc &desc)
{

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = device->m_vkb_device.get_queue_index(vkb::QueueType::graphics).value();


    VkCommandPool command_pool;
    if (vkCreateCommandPool(device->m_vkb_device, &poolInfo, nullptr, &command_pool) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan command pool");
        return nullptr;
    }

    gfx::CommandPoolHandle *handle = static_cast<gfx::CommandPoolHandle *>(HV_NEW(sizeof(gfx::CommandPoolHandle), HIVE_ALLOC_CAT_GRAPHIC));

    handle->command_pool = command_pool;
    return handle;
}

void hive::vk::vulkan_command_pool_destroy(const hive::gfx::DeviceHandle *device,
    hive::gfx::CommandPoolHandle *command_pool)
{
    vkDestroyCommandPool(device->m_vkb_device, command_pool->command_pool, nullptr);
    HV_FREE(command_pool);
}

hive::gfx::CommandBufferHandle* hive::vk::vulkan_command_buffer_allocate(const hive::gfx::DeviceHandle* device,
    const hive::gfx::CommandBufferDesc& desc)
{
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = desc.command_pool->command_pool;
    alloc_info.commandBufferCount = desc.count;

    std::vector<VkCommandBuffer> command_buffers;
    command_buffers.resize(desc.count);
    if (vkAllocateCommandBuffers(device->m_vkb_device, &alloc_info, command_buffers.data()) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to allocate vulkan command buffers");
        return nullptr;
    }

    auto *handle = static_cast<gfx::CommandBufferHandle*>(HV_NEW(sizeof(gfx::CommandBufferHandle), HIVE_ALLOC_CAT_GRAPHIC));
    handle->command_buffers = std::move(command_buffers);

    return handle;
}

void hive::vk::vulkan_command_buffer_free(const hive::gfx::DeviceHandle* device,
                                          gfx::CommandPoolHandle* command_pool, gfx::CommandBufferHandle* command_buffer)
{
    vkFreeCommandBuffers(device->m_vkb_device, command_pool->command_pool, command_buffer->command_buffers.size(), command_buffer->command_buffers.data());
    HV_FREE(command_buffer);
}

VkCommandBuffer hive::vk::beginSingleTimeCommands(const gfx::DeviceHandle* device, gfx::CommandPoolHandle *command_pool)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = command_pool->command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device->m_vkb_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void hive::vk::endSingleTimeCommands(const gfx::DeviceHandle* device, gfx::CommandPoolHandle *command_pool, VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->m_vkb_device.get_queue(vkb::QueueType::graphics).value(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->m_vkb_device.get_queue(vkb::QueueType::graphics).value());

    vkFreeCommandBuffers(device->m_vkb_device, command_pool->command_pool, 1, &commandBuffer);
}
