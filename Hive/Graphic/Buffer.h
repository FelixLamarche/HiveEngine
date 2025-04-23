#pragma once
#include "API_utils.h"

namespace hive::gfx
{
    struct GraphicDevice;
    struct CommandPoolHandle;

    struct BufferHandle;

    enum BufferUsageFlagBits
    {
        BUFFER_USAGE_TRANSFER_DST = 1 << 0,
        BUFFER_USAGE_TRANSFER_SRC = 1 << 1,
        BUFFER_USAGE_UNIFORM_BUFFER = 1 << 2,
        BUFFER_USAGE_INDEX_BUFFER = 1 << 3,
        BUFFER_USAGE_VERTEX_BUFFER = 1 << 4,
    };

    struct BufferDesc
    {
        uint32 size;
        uint32 usage_flags;
        uint32 memory_properties_flags;
    };

#define BUFFER_API(GFX) \
    GFX(hive::gfx::BufferHandle*, buffer_create, (const hive::gfx::GraphicDevice* device, const hive::gfx::BufferDesc& desc), device, desc) \
    GFX(void, buffer_destroy, (const hive::gfx::GraphicDevice* device, hive::gfx::BufferHandle *buffer), device, buffer) \
    GFX(void, buffer_update, (const hive::gfx::GraphicDevice* device, hive::gfx::CommandPoolHandle *command_pool, hive::gfx::BufferHandle *buffer, const void* data, size_t size), device, command_pool, buffer, data, size) \



    struct BufferAPI
    {
        BUFFER_API(DEFINE_FN_PTR)
    };

    BUFFER_API(DEFINE_FREE_FN)
    BUFFER_API(DEFINE_EXTERN_FN_PTR)

}
