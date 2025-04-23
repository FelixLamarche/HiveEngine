#include "Graphic/Swapchain.h"
#include "Graphic/Shader.h"
#include "Graphic/RenderPass.h"
#include "GraphicAPI.h"

#include "Buffer.h"
#include "Command.h"
#include "Framebuffer.h"
#include "Texture.h"

std::vector<hive::gfx::GraphicAPI*> g_graphic_apis;
hive::gfx::GraphicAPI* g_current_graphic_api;


void hive::gfx::RegisterAPI(GraphicAPI* api)
{
    g_graphic_apis.push_back(api);
}

void SetupSwapchainAPI(const hive::gfx::SwapchainAPI* api);
void SetupShaderAPI(const hive::gfx::ShaderAPI* api);
void SetupRenderpassAPI(const hive::gfx::RenderpassAPI *api);
void SetupCommandAPI(const hive::gfx::CommandAPI *api);
void SetupTextureAPI(const hive::gfx::TextureAPI *api);
void SetupFramebufferAPI(const hive::gfx::FramebufferAPI *api);
void SetupBufferAPI(const hive::gfx::BufferAPI *api);



void hive::gfx::SetBackend(const std::string &backend)
{
    if (g_current_graphic_api != nullptr)
    {
        HIVE_LOG_WARN("Attention changing the backend might cause some serious issue with already allocated GraphicDevice");
    }

    for (auto api : g_graphic_apis)
    {
        if (api->name == backend)
        {
            g_current_graphic_api = api;
            SetupSwapchainAPI(g_current_graphic_api->swapchain_api);
            SetupShaderAPI(g_current_graphic_api->shader_api);
            SetupRenderpassAPI(g_current_graphic_api->renderpass_api);
            SetupCommandAPI(g_current_graphic_api->command_api);
            SetupTextureAPI(g_current_graphic_api->texture_api);
            SetupFramebufferAPI(g_current_graphic_api->framebuffer_api);
            SetupBufferAPI(g_current_graphic_api->buffer_api);

            //TODO setup the PFN function for all the api
        }
    }

    if (g_current_graphic_api == nullptr)
    {
        HIVE_LOG_ERROR("Failed to find the requested backend");
    }
}

hive::gfx::GraphicDevice* hive::gfx::device_create(const DeviceCreateInfo& create_info)
{
    return g_current_graphic_api->CreateDevice(create_info);
}

void hive::gfx::device_destroy(GraphicDevice* device)
{
    g_current_graphic_api->DestroyDevice(device);
}

void SetupSwapchainAPI(const hive::gfx::SwapchainAPI* api)
{
    //TODO make this more automated
    hive::gfx::PFN_swapchain_create = api->PFN_swapchain_create;
    hive::gfx::PFN_swapchain_destroy = api->PFN_swapchain_destroy;
    hive::gfx::PFN_swapchain_resize = api->PFN_swapchain_resize;
}

void SetupShaderAPI(const hive::gfx::ShaderAPI *api)
{
    //TODO make this more automated
    hive::gfx::PFN_shader_create = api->PFN_shader_create;
    hive::gfx::PFN_shader_destroy = api->PFN_shader_destroy;
    hive::gfx::PFN_shader_program_create = api->PFN_shader_program_create;
    hive::gfx::PFN_shader_program_destroy = api->PFN_shader_program_destroy;
}

void SetupRenderpassAPI(const hive::gfx::RenderpassAPI *api)
{
    hive::gfx::PFN_renderpass_create = api->PFN_renderpass_create;
    hive::gfx::PFN_renderpass_destroy = api->PFN_renderpass_destroy;
}

void SetupCommandAPI(const hive::gfx::CommandAPI *api)
{
    hive::gfx::PFN_command_pool_create = api->PFN_command_pool_create;
    hive::gfx::PFN_command_pool_destroy = api->PFN_command_pool_destroy;
}

void SetupTextureAPI(const hive::gfx::TextureAPI *api)
{
    hive::gfx::PFN_texture_create = api->PFN_texture_create;
    hive::gfx::PFN_texture_destroy = api->PFN_texture_destroy;
    hive::gfx::PFN_texture_copy = api->PFN_texture_copy;
    hive::gfx::PFN_texture_update = api->PFN_texture_update;
}

void SetupFramebufferAPI(const hive::gfx::FramebufferAPI *api)
{
    hive::gfx::PFN_framebuffer_create = api->PFN_framebuffer_create;
    hive::gfx::PFN_framebuffer_destroy = api->PFN_framebuffer_destroy;
}

void SetupBufferAPI(const hive::gfx::BufferAPI* api)
{
    hive::gfx::PFN_buffer_create = api->PFN_buffer_create;
    hive::gfx::PFN_buffer_destroy = api->PFN_buffer_destroy;
    hive::gfx::PFN_buffer_update = api->PFN_buffer_update;
}

