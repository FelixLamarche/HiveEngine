find_package(Vulkan REQUIRED)
set(VK_BOOTSTRAP_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(3rdparty/vk-bootstrap)


add_library(Vulkan SHARED Module.cpp
        BufferVulkan.cpp
        CommandVulkan.cpp
        FramebufferVulkan.cpp
        GraphicDeviceVulkan.cpp
        RenderpassVulkan.cpp
        ShaderVulkan.cpp
        SurfaceFactory.cpp
        SwapchainVulkan.cpp
        SyncVulkan.cpp
        TextureVulkan.cpp
        UtilsVulkan.cpp
)

target_link_libraries(Vulkan PRIVATE vk-bootstrap HiveCore Vulkan::Vulkan)
target_compile_definitions(Vulkan PRIVATE HIVE_EXPORTS)
