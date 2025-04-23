#pragma once
#include "API_utils.h"

namespace hive::gfx
{
    struct GraphicDevice;

    struct CommandPoolHandle;
    struct CommandPoolDesc
    {
    };

#define COMMAND_API(GFX) \
    GFX(hive::gfx::CommandPoolHandle *, command_pool_create, (const hive::gfx::GraphicDevice *device, const hive::gfx::CommandPoolDesc &desc), device, desc) \
    GFX(void, command_pool_destroy, (const hive::gfx::GraphicDevice *device, hive::gfx::CommandPoolHandle *command_pool), device, command_pool)


    struct CommandAPI
    {
        COMMAND_API(DEFINE_FN_PTR)
    };
    COMMAND_API(DEFINE_EXTERN_FN_PTR)
    COMMAND_API(DEFINE_FREE_FN)
}
