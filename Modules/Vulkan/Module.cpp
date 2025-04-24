#include "Graphic/GFX.h"

#include "BufferVulkan.h"
#include "CommandVulkan.h"

#include "GraphicDeviceVulkan.h"
#include "RenderpassVulkan.h"
#include "ShaderVulkan.h"
#include "SwapchainVulkan.h"
#include "TextureVulkan.h"
#include "FramebufferVulkan.h"
#include "SyncVulkan.h"


void setup_vk_api(hive::gfx::GFX_API *api);


hive::gfx::GFX_API *vk_api = nullptr;
extern "C" HIVE_API void InitModule()
{
    HIVE_LOG_DEBUG("Init vulkan module");
    vk_api = static_cast<hive::gfx::GFX_API*>(HV_NEW(sizeof(hive::gfx::GFX_API), HIVE_ALLOC_CAT_GRAPHIC));
    setup_vk_api(vk_api);
    hive::gfx::register_api(vk_api);
}

extern "C" HIVE_API void ShutdownModule()
{
    HV_FREE(vk_api);
    HIVE_LOG_DEBUG("Shutdown vulkan module");
}

#define SETUP_VK_API_FN_PTR(ReturnType, Name, ParamList, ...) api->PFN_##Name = hive::vk::vulkan_##Name;
void setup_vk_api(hive::gfx::GFX_API *api)
{
    api->name = "vulkan";
    DEVICE_API(SETUP_VK_API_FN_PTR);
    SWAPCHAIN_API(SETUP_VK_API_FN_PTR);
    RENDERPASS_API(SETUP_VK_API_FN_PTR);
    SHADER_API(SETUP_VK_API_FN_PTR);
    TEXTURE_API(SETUP_VK_API_FN_PTR);
    FRAMEBUFFER_API(SETUP_VK_API_FN_PTR);
    COMMAND_API(SETUP_VK_API_FN_PTR);
    BUFFER_API(SETUP_VK_API_FN_PTR);
    SYNC_API(SETUP_VK_API_FN_PTR);
}
