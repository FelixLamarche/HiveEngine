#pragma once
namespace hive
{
    struct Display;
}
namespace hive::gfx
{
    //================= Device ===============
    struct DeviceDesc
    {
        Display* display;
    };
    struct DeviceHandle;

    #define DEVICE_API(GFX) \
            GFX(hive::gfx::DeviceHandle*, device_create, (const hive::gfx::DeviceDesc &desc), desc) \
            GFX(void, device_destroy, (hive::gfx::DeviceHandle *device), device) \
            GFX(void, device_wait_idle, (const hive::gfx::DeviceHandle *device), device) \



    enum MemoryPropertyFlagBits
    {
        MEMORY_PROPERTY_DEVICE_LOCAL = 1 << 0,
        MEMORY_PROPERTY_HOST_VISIBLE_BIT = 1 << 1,
        MEMORY_PROPERTY_HOST_COHERENT_BIT = 1 << 2,
    };

    //================= Swapchain ===============
    struct SwapchainDesc
    {
    };
    struct SwapchainHandle;

    #define SWAPCHAIN_API(GFX) \
        GFX(hive::gfx::SwapchainHandle*, swapchain_create, (const hive::gfx::DeviceHandle* device, const hive::gfx::SwapchainDesc& desc), device, desc) \
        GFX(void, swapchain_destroy, (const hive::gfx::DeviceHandle* device, hive::gfx::SwapchainHandle* swapchain), device, swapchain) \
        GFX(hive::gfx::SwapchainHandle*, swapchain_resize, (const hive::gfx::DeviceHandle* device, hive::gfx::SwapchainHandle* prev_swapchain, uint16 width, uint16 height), device, prev_swapchain, width, height) \


    //================= Renderpass ===============
    struct RenderpassDesc
    {
        SwapchainHandle *swapchain;
    };

    struct RenderpassHandle;

    #define RENDERPASS_API(GFX) \
        GFX(hive::gfx::RenderpassHandle*, renderpass_create, (const hive::gfx::DeviceHandle *device, const hive::gfx::RenderpassDesc &desc), device, desc) \
        GFX(void, renderpass_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::RenderpassHandle *render_pass), device, render_pass) \

    //================= Texture ===============
    enum TextureFormat
    {
        TEXTURE_FORMAT_D32_SFLOAT,
        TEXTURE_FORMAT_B8G8R8A8_UNORM,
        TEXTURE_FORMAT_R8G8B8A8_UNORM,
        TEXTURE_FORMAT_R32G32B32_SFLOAT,
        TEXTURE_FORMAT_R32G32_SFLOAT,
        TEXTURE_FORMAT_R8_UNORM,
        TEXTURE_FORMAT_R8G8B8A8_SRGB
    };

    enum TextureTiling
    {
        TEXTURE_TILING_OPTIMAL,
        TEXTURE_TILING_LINEAR,
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
        TextureFormat format;
        TextureTiling tiling;

        uint32 usage_flags;
        uint32 memory_property_flags;
        uint32 aspect_flags;

        uint32 width;
        uint32 height;
    };

    struct TextureHandle;

    #define TEXTURE_API(GFX) \
        GFX(hive::gfx::TextureHandle*, texture_create, (const hive::gfx::DeviceHandle* device, const hive::gfx::TextureDesc& desc), device, desc) \
        GFX(void, texture_update, (const hive::gfx::DeviceHandle* device, hive::gfx::CommandPoolHandle *command_pool, hive::gfx::TextureHandle* texture, const void* data, size_t size), device, command_pool, texture, data, size) \
        GFX(void, texture_destroy, (const hive::gfx::DeviceHandle* device, hive::gfx::TextureHandle* texture), device, texture) \
        GFX(hive::gfx::TextureHandle*, texture_copy, (const hive::gfx::DeviceHandle* device, hive::gfx::TextureHandle* texture), device, texture) \


    //================= Buffer ===============
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
        uint32 memory_property_flags;
    };

    struct BufferHandle;

    #define BUFFER_API(GFX) \
        GFX(hive::gfx::BufferHandle*, buffer_create, (const hive::gfx::DeviceHandle* device, const hive::gfx::BufferDesc& desc), device, desc) \
        GFX(void, buffer_destroy, (const hive::gfx::DeviceHandle* device, hive::gfx::BufferHandle *buffer), device, buffer) \
        GFX(void, buffer_update, (const hive::gfx::DeviceHandle* device, hive::gfx::CommandPoolHandle *command_pool, hive::gfx::BufferHandle *buffer, const void* data, size_t size), device, command_pool, buffer, data, size) \

    //================= Shader ===============
    enum ShaderType
    {
        SHADER_TYPE_VERTEX,
        SHADER_TYPE_FRAGMENT,
        SHADER_TYPE_GEOMETRY
    };

    enum DescriptorType
    {
        DESCRIPTOR_TYPE_UBO,
        DESCRIPTOR_TYPE_SAMPLER
    };

    struct BindingGroupLayoutDesc
    {
        uint32 binding;
        uint32 count;
        DescriptorType type;
        ShaderType shader_type;
    };
    struct BindingGroupLayoutHandle; //VkDescriptorLayout

    struct BindingPoolDesc
    {
        uint32 max_sets;
        std::vector<BindingGroupLayoutDesc> layouts;
    };
    struct BindingPoolHandle; //VkDescriptorPool

    struct BindingGroupDesc
    {
        BindingPoolHandle *binding_pool;
        BindingGroupLayoutHandle* binding_layout;
        uint32 count;
    };
    struct BindingGroupHandle; //VkDescriptorSet

    struct BindingGroupWriteDesc
    {
        union {
            TextureHandle *texture;
            BufferHandle *buffer;
        };
        BindingGroupHandle *dst_binding_group;
        uint32 binding;
        uint32 count;
        DescriptorType type;
    };

    struct ShaderDesc
    {
        const char* path;
        ShaderType shader_type;
    };

    struct ShaderHandle;


    struct ShaderProgramDesc
    {
        BindingGroupLayoutHandle *bindings;
        std::vector<ShaderHandle*> shader_handles;
        RenderpassHandle *render_pass;
    };

    struct ShaderProgramHandle;

    #define SHADER_API(GFX) \
        GFX(hive::gfx::ShaderHandle*, shader_create, (const hive::gfx::DeviceHandle *device, const hive::gfx::ShaderDesc &desc), device, desc) \
        GFX(void, shader_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::ShaderHandle *shader), device, shader) \
        GFX(hive::gfx::ShaderProgramHandle*, shader_program_create, (const hive::gfx::DeviceHandle *device, const hive::gfx::ShaderProgramDesc &desc), device, desc) \
        GFX(void, shader_program_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::ShaderProgramHandle *shader_program), device, shader_program) \
        GFX(hive::gfx::BindingGroupLayoutHandle*, binding_group_layout_create, (const hive::gfx::DeviceHandle *device, const std::vector<hive::gfx::BindingGroupLayoutDesc> &desc), device, desc) \
        GFX(void, binding_group_layout_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::BindingGroupLayoutHandle *binding_group_layout), device, binding_group_layout) \
        GFX(hive::gfx::BindingPoolHandle*, binding_pool_create, (const hive::gfx::DeviceHandle *device, const hive::gfx::BindingPoolDesc &desc), device, desc) \
        GFX(void, binding_pool_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::BindingPoolHandle *binding_pool), device, binding_pool) \
        GFX(hive::gfx::BindingGroupHandle*, binding_group_allocate, (const hive::gfx::DeviceHandle *device, const hive::gfx::BindingGroupDesc &desc), device, desc) \
        GFX(void, binding_group_free, (const hive::gfx::DeviceHandle *device, hive::gfx::BindingPoolHandle *binding_pool, hive::gfx::BindingGroupHandle *binding_group), device, binding_pool, binding_group) \
        GFX(void, binding_group_write, (const hive::gfx::DeviceHandle *device, const hive::gfx::BindingGroupWriteDesc &desc), device, desc) \
        // GFX(void, binding_group_write, ())
        // GFX(hive::gfx::BindingGroupHandle*, binding_group_create, (const hive::gfx::DeviceHandle *device, const hive::gfx::BindingGroupDesc &desc), device, desc) \
        // GFX(void, binding_group_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::BindingGroupHandle *binding_group), device, binding_group) \

    //================= Framebuffer ===============
    struct FramebufferDesc
    {
        SwapchainHandle *swapchain;
        RenderpassHandle *render_pass;
        TextureHandle *depth_texture = nullptr; //Optional
    };

    struct FramebufferHandle;

    #define FRAMEBUFFER_API(GFX) \
        GFX(hive::gfx::FramebufferHandle*, framebuffer_create, (const hive::gfx::DeviceHandle* device, const hive::gfx::FramebufferDesc &desc), device, desc) \
        GFX(void, framebuffer_destroy, (const hive::gfx::DeviceHandle* device, hive::gfx::FramebufferHandle* framebuffer), device, framebuffer) \



    //================= Command ===============
    struct CommandPoolDesc
    {

    };
    struct CommandPoolHandle;

    struct CommandBufferDesc
    {
        CommandPoolHandle *command_pool;
        uint32 count;
    };
    struct CommandBufferHandle;

    #define COMMAND_API(GFX) \
        GFX(hive::gfx::CommandPoolHandle *, command_pool_create, (const hive::gfx::DeviceHandle *device, const hive::gfx::CommandPoolDesc &desc), device, desc) \
        GFX(void, command_pool_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::CommandPoolHandle *command_pool), device, command_pool) \
        GFX(hive::gfx::CommandBufferHandle*, command_buffer_allocate, (const hive::gfx::DeviceHandle *device, const hive::gfx::CommandBufferDesc &desc), device, desc) \
        GFX(void, command_buffer_free, (const hive::gfx::DeviceHandle *device, hive::gfx::CommandPoolHandle *command_pool, hive::gfx::CommandBufferHandle *command_buffer), device, command_pool, command_buffer)




    //================= Sync ===============

    struct SemaphoreHandle;
    struct FenceHandle;

    #define SYNC_API(GFX) \
        GFX(hive::gfx::SemaphoreHandle*, semaphore_create, (const hive::gfx::DeviceHandle *device), device) \
        GFX(void, semaphore_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::SemaphoreHandle *semaphore), device, semaphore) \
        GFX(hive::gfx::FenceHandle*, fence_create, (const hive::gfx::DeviceHandle *device, bool is_signaled), device, is_signaled) \
        GFX(void, fence_destroy, (const hive::gfx::DeviceHandle *device, hive::gfx::FenceHandle *fence), device, fence) \
        GFX(void, tmp_draw, (int current_frame, const hive::gfx::DeviceHandle *device, hive::gfx::FenceHandle *fence, hive::gfx::SemaphoreHandle *image_availabl_sem, hive::gfx::SemaphoreHandle *render_finished_sem, hive::gfx::SwapchainHandle *swapchain, hive::gfx::CommandBufferHandle *command_buffer_handle, hive::gfx::FramebufferHandle *framebuffer, hive::gfx::RenderpassHandle *render_pass, hive::gfx::ShaderProgramHandle *shader_program_handle, hive::gfx::BufferHandle *vertex_buffer, hive::gfx::BufferHandle *index_buffer, hive::gfx::BindingGroupHandle *binding_group, int indices_count), current_frame, device, fence, image_availabl_sem, render_finished_sem, swapchain, command_buffer_handle, framebuffer, render_pass, shader_program_handle, vertex_buffer, index_buffer, binding_group, indices_count) \


    //================= API ===============
    #define DEFINE_FREE_FN(ReturnType, Name, ParamList, ...) HIVE_API ReturnType Name ParamList;

    #define DEFINE_FN_PTR(ReturnType, Name, ParamList, ...) ReturnType (*PFN_##Name) ParamList;

    #define DEFINE_EXTERN_FN_PTR(ReturnType, Name, ParamList, ...) extern ReturnType (*PFN_##Name) ParamList;

    #define IMPL_EXTERN_FN_PTR(ReturnType, Name, ParamList, ...) ReturnType (*PFN_##Name) ParamList = nullptr;

    #define IMPL_FREE_FN(ReturnType, Name, ParamList, ...) \
        ReturnType Name ParamList { \
            HIVE_ASSERT(PFN_##Name != nullptr, "The function is not setup"); \
            return PFN_##Name(__VA_ARGS__); \
        }

    #define SETUP_API_FN_PTR(ReturnType, Name, ParamList, ...) PFN_##Name = api->PFN_##Name;


    struct GFX_API
    {
        const char* name;

        DEVICE_API(DEFINE_FN_PTR);
        SWAPCHAIN_API(DEFINE_FN_PTR);
        RENDERPASS_API(DEFINE_FN_PTR);
        SHADER_API(DEFINE_FN_PTR);
        TEXTURE_API(DEFINE_FN_PTR);
        FRAMEBUFFER_API(DEFINE_FN_PTR);
        COMMAND_API(DEFINE_FN_PTR);
        BUFFER_API(DEFINE_FN_PTR);
        SYNC_API(DEFINE_FN_PTR);
    };

    DEVICE_API(DEFINE_FREE_FN);
    SWAPCHAIN_API(DEFINE_FREE_FN);
    RENDERPASS_API(DEFINE_FREE_FN);
    SHADER_API(DEFINE_FREE_FN);
    TEXTURE_API(DEFINE_FREE_FN);
    FRAMEBUFFER_API(DEFINE_FREE_FN);
    COMMAND_API(DEFINE_FREE_FN);
    BUFFER_API(DEFINE_FREE_FN);
    SYNC_API(DEFINE_FREE_FN);

    DEVICE_API(DEFINE_EXTERN_FN_PTR);
    SWAPCHAIN_API(DEFINE_EXTERN_FN_PTR);
    RENDERPASS_API(DEFINE_EXTERN_FN_PTR);
    SHADER_API(DEFINE_EXTERN_FN_PTR);
    TEXTURE_API(DEFINE_EXTERN_FN_PTR);
    FRAMEBUFFER_API(DEFINE_EXTERN_FN_PTR);
    COMMAND_API(DEFINE_EXTERN_FN_PTR);
    BUFFER_API(DEFINE_EXTERN_FN_PTR);
    SYNC_API(DEFINE_EXTERN_FN_PTR);

    HIVE_API void register_api(const GFX_API *api);
    HIVE_API void set_backend_api(const std::string &backend);
}
