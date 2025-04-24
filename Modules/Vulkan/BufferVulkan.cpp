//
// Created by samuel on 23/04/25.
//

#include "BufferVulkan.h"
#include "Graphic/GFX.h"
#include "GraphicDeviceVulkan.h"
#include "UtilsVulkan.h"
#include "CommandVulkan.h"

VkBufferUsageFlags get_buffer_usage(const uint32 usage)
{
    VkBufferUsageFlags flag = 0;

    if (usage & hive::gfx::BufferUsageFlagBits::BUFFER_USAGE_INDEX_BUFFER)
    {
        flag |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }

    if (usage & hive::gfx::BufferUsageFlagBits::BUFFER_USAGE_VERTEX_BUFFER)
    {
        flag |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }

    if (usage & hive::gfx::BufferUsageFlagBits::BUFFER_USAGE_UNIFORM_BUFFER)
    {
        flag |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }

    if (usage & hive::gfx::BufferUsageFlagBits::BUFFER_USAGE_TRANSFER_DST)
    {
        flag |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    if (usage & hive::gfx::BufferUsageFlagBits::BUFFER_USAGE_TRANSFER_SRC)
    {
        flag |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    }


    return flag;
}

void hive::vk::createBuffer(const gfx::DeviceHandle* device, VkDeviceSize size, VkBufferUsageFlags usage,
                            VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device->m_vkb_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device->m_vkb_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(device, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device->m_vkb_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device->m_vkb_device, buffer, bufferMemory, 0);
}

hive::gfx::BufferHandle* hive::vk::vulkan_buffer_create(const hive::gfx::DeviceHandle* device,
    const hive::gfx::BufferDesc& desc)
{
    VkBufferCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    create_info.size = desc.size;
    create_info.usage = get_buffer_usage(desc.usage_flags);
    create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer = VK_NULL_HANDLE;
    if (vkCreateBuffer(device->m_vkb_device, &create_info, nullptr, &buffer) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan buffer");
        return nullptr;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device->m_vkb_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(device, memRequirements.memoryTypeBits, desc.memory_property_flags);

    VkDeviceMemory memory = VK_NULL_HANDLE;
    if (vkAllocateMemory(device->m_vkb_device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to allocate vulkan memory!");
        return nullptr;
    }


    vkBindBufferMemory(device->m_vkb_device, buffer, memory, 0);

    gfx::BufferHandle* handle = static_cast<gfx::BufferHandle*>(HV_NEW(sizeof(gfx::BufferHandle), HIVE_ALLOC_CAT_GRAPHIC));
    handle->buffer = buffer;
    handle->buffer_info = create_info;
    handle->memory = memory;

    return handle;
}

void hive::vk::vulkan_buffer_destroy(const hive::gfx::DeviceHandle* device, hive::gfx::BufferHandle* buffer)
{
    vkFreeMemory(device->m_vkb_device, buffer->memory, nullptr);
    vkDestroyBuffer(device->m_vkb_device, buffer->buffer, nullptr);

    HV_FREE(buffer);
}

void hive::vk::vulkan_buffer_update(const hive::gfx::DeviceHandle* device, hive::gfx::CommandPoolHandle * command_pool, hive::gfx::BufferHandle* buffer,
                                    const void* data, size_t size)
{
    gfx::BufferDesc desc{};
    desc.usage_flags = gfx::BUFFER_USAGE_TRANSFER_SRC;
    desc.memory_property_flags = gfx::MEMORY_PROPERTY_HOST_VISIBLE_BIT | gfx::MEMORY_PROPERTY_HOST_COHERENT_BIT;
    desc.size = size;

    auto tmp_buffer = vk::vulkan_buffer_create(device, desc);

    void* data_handle;
    vkMapMemory(device->m_vkb_device, tmp_buffer->memory, 0, size, 0, &data_handle);
    memcpy(data_handle, data, size);
    vkUnmapMemory(device->m_vkb_device, tmp_buffer->memory);

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, command_pool);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, tmp_buffer->buffer, buffer->buffer, 1, &copyRegion);

    endSingleTimeCommands(device, command_pool, commandBuffer);

    vk::vulkan_buffer_destroy(device, tmp_buffer);
}
