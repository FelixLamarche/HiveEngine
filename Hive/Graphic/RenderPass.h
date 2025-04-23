#pragma once
#include "API_utils.h"

namespace hive::gfx
{
    struct GraphicDevice;
    struct SwapchainHandle;

    struct RenderPassHandle;

    struct RenderpassDesc
    {
        SwapchainHandle* swapchain;
    };

#define RENDERPASS_API(GFX) \
    GFX(hive::gfx::RenderPassHandle*, renderpass_create, (const hive::gfx::GraphicDevice *device, const hive::gfx::RenderpassDesc &desc), device, desc) \
    GFX(void, renderpass_destroy, (const hive::gfx::GraphicDevice *device, hive::gfx::RenderPassHandle *render_pass), device, render_pass) \

    struct RenderpassAPI
    {
        RENDERPASS_API(DEFINE_FN_PTR)
    };

    RENDERPASS_API(DEFINE_FREE_FN)
    RENDERPASS_API(DEFINE_EXTERN_FN_PTR)
}
