#pragma once
#include "Graphic/GFX.h"
#include "vulkan/vulkan.h"

namespace hive::gfx
{
    struct TextureHandle
    {
        VkImage image;
        VkImageView image_view;
        VkDeviceMemory memory;
        VkSampler sampler;

        VkImageCreateInfo image_info;
        VkImageViewCreateInfo image_view_info;
    };
}
namespace hive::vk
{
    hive::gfx::TextureHandle * vulkan_texture_create(const hive::gfx::DeviceHandle *device,
                                                     const hive::gfx::TextureDesc &desc);

    void vulkan_texture_update(const hive::gfx::DeviceHandle *device, hive::gfx::CommandPoolHandle *command_pool, hive::gfx::TextureHandle *texture,
                               const void *data, size_t size);

    void vulkan_texture_destroy(const hive::gfx::DeviceHandle *device, hive::gfx::TextureHandle *texture);

    hive::gfx::TextureHandle *vulkan_texture_copy(const hive::gfx::DeviceHandle *device,
                                                  hive::gfx::TextureHandle *texture);



}
