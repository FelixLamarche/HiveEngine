#pragma once
#include "API_utils.h"

namespace hive::gfx
{
    struct RenderPassHandle;
    struct TextureHandle;
    struct SwapchainHandle;
    struct GraphicDevice;
}



namespace hive::gfx
{

    struct FramebufferHandle;
    struct FramebufferDesc
    {
        SwapchainHandle *swapchain = nullptr;
        RenderPassHandle *render_pass = nullptr;
        TextureHandle *depth_texture = nullptr; //Optional
    };

#define FRAMEBUFFER_API(GFX) \
    GFX(hive::gfx::FramebufferHandle*, framebuffer_create, (const hive::gfx::GraphicDevice* device, const hive::gfx::FramebufferDesc &desc), device, desc) \
    GFX(void, framebuffer_destroy, (const hive::gfx::GraphicDevice* device, hive::gfx::FramebufferHandle* framebuffer), device, framebuffer) \

    struct FramebufferAPI
    {
        FRAMEBUFFER_API(DEFINE_FN_PTR)
    };

    FRAMEBUFFER_API(DEFINE_EXTERN_FN_PTR)
    FRAMEBUFFER_API(DEFINE_FREE_FN)

}
