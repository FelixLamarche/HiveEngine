#pragma once

namespace hive::gfx
{
    struct BufferAPI;
    struct FramebufferAPI;
    struct CommandAPI;
    struct SwapchainAPI;
    struct RenderpassAPI;
    struct ShaderAPI;
    struct TextureAPI;
}

namespace hive
{
    class Display;
}

namespace hive::gfx
{
    //All of those type are opaque handle of the actual resource;

    /**
     * @brief Represents the main interface to the GPU.
     *
     * The `GraphicDevice` is responsible for resource creation, command submission,
     * and synchronization with the GPU.
     */
    struct GraphicDevice;




    struct DeviceCreateInfo
    {
        Display *display = nullptr;
        DeviceCreateInfo(Display *display_ptr) : display(display_ptr) {}
    };

    struct GraphicAPI
    {
        const std::string name;
        GraphicDevice* (*CreateDevice)(const DeviceCreateInfo &create_info);
        void (*DestroyDevice)(GraphicDevice *device);

        TextureAPI *texture_api = nullptr;
        SwapchainAPI *swapchain_api = nullptr;
        ShaderAPI *shader_api = nullptr;
        RenderpassAPI *renderpass_api = nullptr;
        CommandAPI *command_api = nullptr;
        FramebufferAPI *framebuffer_api = nullptr;
        BufferAPI *buffer_api = nullptr;
    };

}
