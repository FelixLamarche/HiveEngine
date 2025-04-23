#pragma once
#include "API_utils.h"

namespace hive::gfx
{
    struct GraphicDevice;
    struct CommandPoolHandle;
}

namespace hive::gfx
{
    struct TextureHandle;

    enum class TextureFormat
    {
        TEXTURE_FORMAT_D32_SFLOAT,
        TEXTURE_FORMAT_B8G8R8A8_UNORM,
        TEXTURE_FORMAT_R8G8B8A8_UNORM,
        TEXTURE_FORMAT_R32G32B32_SFLOAT,
        TEXTURE_FORMAT_R32G32_SFLOAT,
        TEXTURE_FORMAT_R8_UNORM,
        TEXTURE_FORMAT_R8G8B8A8_SRGB
    };

    enum class TextureTiling
    {
        OPTIMAL, LINEAR,
    };

    enum TextureUsageFlagBits
    {
        TEXTURE_USAGE_SAMPLE  = 1 << 0,
        TEXTURE_USAGE_DEPTH   = 1 << 1,
        TEXTURE_USAGE_COLOR   = 1 << 2,
        TEXTURE_USAGE_SRC     = 1 << 3,
        TEXTURE_USAGE_DST     = 1 << 4,
        TEXTURE_USAGE_STORAGE = 1 << 5
    };

    enum TextureAspectFlagBits
    {
        TEXTURE_ASPECT_COLOR  = 1 << 0,
        TEXTURE_ASPECT_DEPTH   = 1 << 1,
        TEXTURE_ASPECT_STENCIL = 1 << 2,
    };

    struct TextureDesc
    {
        TextureTiling tiling;
        TextureFormat format;

        uint32 usage_flags;
        uint32 memory_property_flags;
        uint32 aspect_flags;

        uint32 width = 0;
        uint32 height = 0;
    };

#define TEXTURE_API(GFX) \
    GFX(hive::gfx::TextureHandle*, texture_create, (const hive::gfx::GraphicDevice* device, const hive::gfx::TextureDesc& desc), device, desc) \
    GFX(void, texture_update, (const hive::gfx::GraphicDevice* device, hive::gfx::CommandPoolHandle *command_pool, hive::gfx::TextureHandle* texture, const void* data, size_t size), device, command_pool, texture, data, size) \
    GFX(void, texture_destroy, (const hive::gfx::GraphicDevice* device, hive::gfx::TextureHandle* texture), device, texture) \
    GFX(hive::gfx::TextureHandle*, texture_copy, (const hive::gfx::GraphicDevice* device, hive::gfx::TextureHandle* texture), device, texture) \

   struct TextureAPI
   {
       TEXTURE_API(DEFINE_FN_PTR)
   };

    TEXTURE_API(DEFINE_FREE_FN)
    TEXTURE_API(DEFINE_EXTERN_FN_PTR)

}
