#include "GFX.h"
#include "Core/Logging/LogManager.h"

namespace hive::gfx
{
    std::vector<const GFX_API*> g_gfx_apis;
    const GFX_API *g_current_api = nullptr;


    void register_api(const GFX_API *api)
    {
        g_gfx_apis.push_back(api);
    }


    void set_api_fn_ptr(const GFX_API *api);
    void set_backend_api(const std::string &backend)
    {
        for(auto api : g_gfx_apis)
        {
            if(api->name == backend)
            {
                g_current_api = api;
                set_api_fn_ptr(g_current_api);
                return;
            }
        }

        HIVE_LOG_ERROR("Failed to find the requested backend");
    }

    void set_api_fn_ptr(const GFX_API *api)
    {
        DEVICE_API(SETUP_API_FN_PTR);
        SWAPCHAIN_API(SETUP_API_FN_PTR);
        RENDERPASS_API(SETUP_API_FN_PTR);
        SHADER_API(SETUP_API_FN_PTR);
        TEXTURE_API(SETUP_API_FN_PTR);
        FRAMEBUFFER_API(SETUP_API_FN_PTR);
        COMMAND_API(SETUP_API_FN_PTR);
        BUFFER_API(SETUP_API_FN_PTR);
        SYNC_API(SETUP_API_FN_PTR);

    }



    DEVICE_API(IMPL_EXTERN_FN_PTR);
    SWAPCHAIN_API(IMPL_EXTERN_FN_PTR);
    RENDERPASS_API(IMPL_EXTERN_FN_PTR);
    SHADER_API(IMPL_EXTERN_FN_PTR);
    TEXTURE_API(IMPL_EXTERN_FN_PTR);
    FRAMEBUFFER_API(IMPL_EXTERN_FN_PTR);
    COMMAND_API(IMPL_EXTERN_FN_PTR);
    BUFFER_API(IMPL_EXTERN_FN_PTR);
    SYNC_API(IMPL_EXTERN_FN_PTR);

    DEVICE_API(IMPL_FREE_FN);
    SWAPCHAIN_API(IMPL_FREE_FN);
    RENDERPASS_API(IMPL_FREE_FN);
    SHADER_API(IMPL_FREE_FN);
    TEXTURE_API(IMPL_FREE_FN);
    FRAMEBUFFER_API(IMPL_FREE_FN);
    COMMAND_API(IMPL_FREE_FN);
    BUFFER_API(IMPL_FREE_FN);
    SYNC_API(IMPL_FREE_FN);
}
