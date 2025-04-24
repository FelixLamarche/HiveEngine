#pragma once
#include "Graphic/GFX.h"
#include "GraphicDeviceVulkan.h"
#include "vulkan/vulkan.h"
namespace hive::gfx
{
    struct ShaderHandle
    {
        VkShaderModule shader_module;
        VkPipelineShaderStageCreateInfo stage_create_info;
    };

    struct ShaderProgramHandle
    {
        VkPipeline pipeline;
        VkPipelineLayout pipeline_layout;

        VkDescriptorPool descriptor_pool;
    };


    struct BindingGroupLayoutHandle
    {
        VkDescriptorSetLayout set_layout;
    };

    struct BindingPoolHandle
    {
        VkDescriptorPool pool;
    };

    struct BindingGroupHandle
    {
        std::vector<VkDescriptorSet> descriptor_sets;
    };
}
namespace hive::vk
{
    gfx::ShaderHandle* vulkan_shader_create(const gfx::DeviceHandle* device, const gfx::ShaderDesc &desc);
    void vulkan_shader_destroy(const gfx::DeviceHandle* device, gfx::ShaderHandle* shader);

    gfx::ShaderProgramHandle* vulkan_shader_program_create(const gfx::DeviceHandle* device, const gfx::ShaderProgramDesc &desc);
    void vulkan_shader_program_destroy(const gfx::DeviceHandle* device, gfx::ShaderProgramHandle* shader);

    gfx::BindingGroupLayoutHandle* vulkan_binding_group_layout_create(const hive::gfx::DeviceHandle *device, const std::vector<hive::gfx::BindingGroupLayoutDesc> &desc);
    void vulkan_binding_group_layout_destroy(const hive::gfx::DeviceHandle *device, hive::gfx::BindingGroupLayoutHandle *binding_group_layout);

    gfx::BindingPoolHandle* vulkan_binding_pool_create(const hive::gfx::DeviceHandle *device, const hive::gfx::BindingPoolDesc &desc);
    void vulkan_binding_pool_destroy(const hive::gfx::DeviceHandle *device, hive::gfx::BindingPoolHandle *binding_pool);

    gfx::BindingGroupHandle* vulkan_binding_group_allocate(const gfx::DeviceHandle *device, const gfx::BindingGroupDesc &desc);
    void vulkan_binding_group_free(const gfx::DeviceHandle *device, gfx::BindingPoolHandle *binding_pool, gfx::BindingGroupHandle *binding_group);
    void vulkan_binding_group_write(const hive::gfx::DeviceHandle *device, const hive::gfx::BindingGroupWriteDesc &desc);
    // void vulkan_binding_group_write()
}
