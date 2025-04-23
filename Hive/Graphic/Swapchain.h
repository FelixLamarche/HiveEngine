#pragma once
#include "API_utils.h"

namespace hive::gfx
{
    struct GraphicDevice;
}

namespace hive::gfx
{
    //Opaque handle for a swapchain
    struct SwapchainHandle;

   struct SwapchainDesc
   {
   };

#define SWAPCHAIN_API(GFX) \
    GFX(hive::gfx::SwapchainHandle*, swapchain_create, (const hive::gfx::GraphicDevice* device, const hive::gfx::SwapchainDesc& desc), device, desc) \
    GFX(void, swapchain_destroy, (const hive::gfx::GraphicDevice* device, hive::gfx::SwapchainHandle* swapchain), device, swapchain) \
    GFX(hive::gfx::SwapchainHandle*, swapchain_resize, (const hive::gfx::GraphicDevice* device, hive::gfx::SwapchainHandle* prev_swapchain, uint16 width, uint16 height), device, prev_swapchain, width, height) \


    struct SwapchainAPI
    {
        SWAPCHAIN_API(DEFINE_FN_PTR)
    };

    SWAPCHAIN_API(DEFINE_FREE_FN)
    SWAPCHAIN_API(DEFINE_EXTERN_FN_PTR)

}

