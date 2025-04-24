#include "Core/Debug.h"
#include "Core/Logging/LogManager.h"
#include "Core/Memory/MemoryAllocator.h"
#include "Graphic/GFX.h"
#include "GraphicDeviceVulkan.h"
#include "RenderpassVulkan.h"
#include "ShaderVulkan.h"
#include <fstream>
#include <vulkan/vulkan_core.h>
#include "BufferVulkan.h"
#include "TextureVulkan.h"


std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        HIVE_LOG_ERROR("Failed to open file at %s", filename.c_str());
        return {};
    }

    const size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
hive::gfx::ShaderHandle * hive::vk::vulkan_shader_create(const gfx::DeviceHandle *device, const gfx::ShaderDesc &desc)
{
    HIVE_LOG_DEBUG("Creating shader handle");

    //TODO: read the file in a buffer
    auto code = readFile(desc.path);

    VkShaderModuleCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());
    create_info.codeSize = code.size();

    VkShaderModule shader_module = VK_NULL_HANDLE;
    if (vkCreateShaderModule(device->m_vkb_device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan shader module!");
        return nullptr;
    }


    const auto handle = static_cast<gfx::ShaderHandle *>(HV_NEW(sizeof(gfx::ShaderHandle), HIVE_ALLOC_CAT_ENGINE));
    handle->shader_module = shader_module;
    handle->stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    handle->stage_create_info.pName = "main";
    handle->stage_create_info.module = shader_module;
    switch (desc.shader_type)
    {
        case gfx::SHADER_TYPE_VERTEX:
            handle->stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case gfx::SHADER_TYPE_FRAGMENT:
            handle->stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        case gfx::SHADER_TYPE_GEOMETRY:
            handle->stage_create_info.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            break;
    }

    return handle;
}

void hive::vk::vulkan_shader_destroy(const gfx::DeviceHandle *device, gfx::ShaderHandle *shader)
{
    HIVE_LOG_DEBUG("Destroying shader handle");
    vkDestroyShaderModule(device->m_vkb_device, shader->shader_module, nullptr);
    HV_FREE(shader);
}

hive::gfx::ShaderProgramHandle * hive::vk::vulkan_shader_program_create(const gfx::DeviceHandle *device,
    const gfx::ShaderProgramDesc &desc)
{
    HIVE_LOG_DEBUG("Creating shader program handle");
    // struct Vertex
    //     {
    //         glm::vec3 pos;
    //         glm::vec3 color;
    //         glm::vec2 texCoord;
    //     };

    VkVertexInputBindingDescription binding_description = {};
    binding_description.binding = 0;
    binding_description.stride = 32; //TODO sizeof(Vertex) or something similar
    binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //Here we describe the vertex layout information (pos, color, texCoord)
    std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions{};
    attribute_descriptions[0].binding = 0;
    attribute_descriptions[0].location = 0;
    attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[0].offset = 0;

    attribute_descriptions[1].binding = 0;
    attribute_descriptions[1].location = 1;
    attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[1].offset = 4 * 3;

    attribute_descriptions[2].binding = 0;
    attribute_descriptions[2].location = 2;
    attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_descriptions[2].offset = 4 * 6;

    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.pVertexBindingDescriptions = &binding_description;
    vertex_input_info.vertexAttributeDescriptionCount = attribute_descriptions.size();
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
    input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_info.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewport_info{};
    viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_info.viewportCount = 1;
    viewport_info.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
                                          | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::array<VkDynamicState, 2> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();


    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &desc.bindings->set_layout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    VkPipelineLayout pipeline_layout;
    if (vkCreatePipelineLayout(device->m_vkb_device, &pipelineLayoutInfo, nullptr, &pipeline_layout) !=
        VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan pipeline layout");
        return nullptr;
    }

    VkPipelineDepthStencilStateCreateInfo depth_stencil{};
    depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil.depthTestEnable = VK_TRUE;
    depth_stencil.depthWriteEnable = VK_TRUE;
    depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depth_stencil.depthBoundsTestEnable = VK_FALSE;
    depth_stencil.minDepthBounds = 0.0f;
    depth_stencil.maxDepthBounds = 1.0f;
    depth_stencil.stencilTestEnable = VK_TRUE;
    depth_stencil.front = {};
    depth_stencil.back = {};

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages{};
    for (auto shader : desc.shader_handles)
    {
        shaderStages.push_back(shader->stage_create_info);
    }


    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertex_input_info;
    pipelineInfo.pInputAssemblyState = &input_assembly_info;
    pipelineInfo.pViewportState = &viewport_info;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipeline_layout;
    pipelineInfo.renderPass = desc.render_pass->render_pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.pDepthStencilState = &depth_stencil;

    VkPipeline pipeline;
    if (vkCreateGraphicsPipelines(device->m_vkb_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        HIVE_LOG_ERROR("Failed to create vulkan graphics pipeline!");
        return nullptr;
    }

    const auto handle = static_cast<gfx::ShaderProgramHandle *>(HV_NEW(sizeof(gfx::ShaderProgramHandle),
                                                                       HIVE_ALLOC_CAT_ENGINE));


    handle->pipeline = pipeline;
    handle->pipeline_layout = pipeline_layout;

    return handle;
}

void hive::vk::vulkan_shader_program_destroy(const gfx::DeviceHandle *device, gfx::ShaderProgramHandle *shader)
{
    HIVE_LOG_DEBUG("Destroying shader program handle");
    HIVE_ASSERT(shader != nullptr, "");
    HIVE_ASSERT(shader->pipeline_layout != VK_NULL_HANDLE, "");
    HIVE_ASSERT(shader->pipeline != VK_NULL_HANDLE, "");

    vkDestroyPipelineLayout(device->m_vkb_device, shader->pipeline_layout, nullptr);
    vkDestroyPipeline(device->m_vkb_device, shader->pipeline, nullptr);

    HV_FREE(shader);
}

hive::gfx::BindingGroupLayoutHandle* hive::vk::vulkan_binding_group_layout_create(const hive::gfx::DeviceHandle *device, const std::vector<hive::gfx::BindingGroupLayoutDesc> &layouts)
{
    std::vector<VkDescriptorSetLayoutBinding> vk_bindings;

    for(const gfx::BindingGroupLayoutDesc &layout : layouts)
    {
        VkDescriptorSetLayoutBinding layout_binding{};
        layout_binding.binding = layout.binding;
        layout_binding.descriptorCount = layout.count;
        layout_binding.pImmutableSamplers = nullptr;

        switch(layout.type)
        {
        case gfx::DESCRIPTOR_TYPE_UBO:
            layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            break;
        case gfx::DESCRIPTOR_TYPE_SAMPLER:
            layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            break;
        }

        switch(layout.shader_type)
        {
        case gfx::SHADER_TYPE_VERTEX:
            layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case gfx::SHADER_TYPE_FRAGMENT:
            layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        case gfx::SHADER_TYPE_GEOMETRY:
            HIVE_ASSERT(false, "Not supported yet");
            break;
        }
        vk_bindings.push_back(layout_binding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(vk_bindings.size());
    layoutInfo.pBindings = vk_bindings.data();

    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    if (vkCreateDescriptorSetLayout(device->m_vkb_device, &layoutInfo, nullptr, &descriptor_set_layout) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to create vulkan binding group layout");
        return nullptr;
    }

    gfx::BindingGroupLayoutHandle *handle = (gfx::BindingGroupLayoutHandle*)HV_NEW(sizeof(gfx::BindingGroupLayoutHandle), HIVE_ALLOC_CAT_GRAPHIC);

    handle->set_layout = descriptor_set_layout;

    return handle;
}

void hive::vk::vulkan_binding_group_layout_destroy(const hive::gfx::DeviceHandle *device, hive::gfx::BindingGroupLayoutHandle *binding_group_layout)
{
    vkDestroyDescriptorSetLayout(device->m_vkb_device, binding_group_layout->set_layout, nullptr);
    HV_FREE(binding_group_layout);
}


hive::gfx::BindingPoolHandle* hive::vk::vulkan_binding_pool_create(const hive::gfx::DeviceHandle *device, const hive::gfx::BindingPoolDesc &desc)
{
    // std::array<VkDescriptorPoolSize, 2> poolSizes{};
    std::vector<VkDescriptorPoolSize> pool_sizes;

    for(auto layout : desc.layouts)
    {
        VkDescriptorPoolSize pool_size{};
        pool_size.descriptorCount = desc.max_sets;
        switch (layout.type)
        {
        case gfx::DESCRIPTOR_TYPE_UBO:
            pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            break;
        case gfx::DESCRIPTOR_TYPE_SAMPLER:
            pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            break;
        }

        pool_sizes.push_back(pool_size);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
    poolInfo.pPoolSizes = pool_sizes.data();
    poolInfo.maxSets = desc.max_sets;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
    if (vkCreateDescriptorPool(device->m_vkb_device, &poolInfo, nullptr, &descriptor_pool) != VK_SUCCESS) {
        HIVE_LOG_ERROR("Failed to create vulkan binding pool");
        return nullptr;
    }

    hive::gfx::BindingPoolHandle *handle = (gfx::BindingPoolHandle*)(HV_NEW(sizeof(gfx::BindingPoolHandle), HIVE_ALLOC_CAT_GRAPHIC));
    handle->pool = descriptor_pool;

    return handle;
}


void hive::vk::vulkan_binding_pool_destroy(const hive::gfx::DeviceHandle *device, hive::gfx::BindingPoolHandle *binding_pool)
{

    vkDestroyDescriptorPool(device->m_vkb_device, binding_pool->pool, nullptr);
    HV_FREE(binding_pool);
}

hive::gfx::BindingGroupHandle* hive::vk::vulkan_binding_group_allocate(const gfx::DeviceHandle *device, const gfx::BindingGroupDesc &desc)
{
    std::vector<VkDescriptorSetLayout> layouts(desc.count, desc.binding_layout->set_layout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = desc.binding_pool->pool;
    allocInfo.descriptorSetCount = desc.count;
    allocInfo.pSetLayouts = layouts.data();

    std::vector<VkDescriptorSet> descriptor_sets;
    descriptor_sets.resize(desc.count);
    if (vkAllocateDescriptorSets(device->m_vkb_device, &allocInfo, descriptor_sets.data()) != VK_SUCCESS)
    {
        HIVE_LOG_ERROR("Failed to allocate vulkan binding group");
        return nullptr;
    }

    gfx::BindingGroupHandle *handle = (gfx::BindingGroupHandle*)HV_NEW(sizeof(gfx::BindingGroupHandle), HIVE_ALLOC_CAT_GRAPHIC);
    handle->descriptor_sets = std::move(descriptor_sets);

    return handle;
}

void hive::vk::vulkan_binding_group_free(const gfx::DeviceHandle *device, gfx::BindingPoolHandle *binding_pool, gfx::BindingGroupHandle *binding_group)
{

    vkFreeDescriptorSets(device->m_vkb_device, binding_pool->pool, binding_group->descriptor_sets.size(), binding_group->descriptor_sets.data());
    HV_FREE(binding_group);
}

void hive::vk::vulkan_binding_group_write(const hive::gfx::DeviceHandle *device, const hive::gfx::BindingGroupWriteDesc &desc)
{
    switch (desc.type)
    {
        case hive::gfx::DESCRIPTOR_TYPE_UBO:
        {
            for(int i = 0; i < desc.count; i++)
            {
                VkDescriptorBufferInfo buffer_info{};
                buffer_info.buffer = desc.buffer->buffer;
                buffer_info.offset = 0;
                buffer_info.range = desc.buffer->buffer_info.size;

                VkWriteDescriptorSet descriptor_write{};
                descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_write.dstSet = desc.dst_binding_group->descriptor_sets[i];
                descriptor_write.dstBinding = desc.binding;
                descriptor_write.dstArrayElement = 0;
                descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptor_write.descriptorCount = 1;
                descriptor_write.pBufferInfo = &buffer_info;

                vkUpdateDescriptorSets(device->m_vkb_device, 1, &descriptor_write, 0, nullptr);
            }
            break;
        }
        case hive::gfx::DESCRIPTOR_TYPE_SAMPLER:
        {
            for(int i = 0; i < desc.count; i++)
            {
                VkDescriptorImageInfo image_info{};
                image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                image_info.imageView = desc.texture->image_view;
                image_info.sampler = desc.texture->sampler;

                VkWriteDescriptorSet descriptor_write{};
                descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_write.dstSet = desc.dst_binding_group->descriptor_sets[i];
                descriptor_write.dstBinding = desc.binding;
                descriptor_write.dstArrayElement = 0;
                descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptor_write.descriptorCount = 1;
                descriptor_write.pImageInfo = &image_info;

                vkUpdateDescriptorSets(device->m_vkb_device, 1, &descriptor_write, 0, nullptr);
            }
            break;
        }
    }
}
