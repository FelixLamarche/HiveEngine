#pragma once

#include "vulkan/vulkan.h"

namespace hive::gfx
{
    struct DeviceHandle;
}


namespace hive::vk
{
    uint32_t findMemoryType(const hive::gfx::DeviceHandle* device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
}
