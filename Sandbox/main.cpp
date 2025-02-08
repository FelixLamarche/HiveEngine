#include <core/Logger.h>
#include <core/Application.h>
#include <rendering/RenderType.h>

#include <iostream>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <rendering/vulkan/GraphicsDeviceVulkan.h>
#include <rendering/vulkan/vulkan_buffer.h>
#include <rendering/vulkan/vulkan_descriptor.h>
#include <rendering/vulkan/vulkan_image.h>
#include <rendering/vulkan/vulkan_pipeline.h>
#include <rendering/vulkan/vulkan_shader.h>
#include <rendering/vulkan/vulkan_types.h>

void RegisterDefaultLoggerSync(hive::Logger::LogLevel level)
{
    hive::Logger::AddSync(level, [](hive::Logger::LogLevel level, const char* msg)
    {
       std::cout << msg << std::endl;
    });
}

class BasicApp final : public hive::Application
{
public:
    explicit BasicApp(hive::ApplicationConfig &config)
        : Application(config)
    {
    }

protected:
    bool on_init() override;

    bool on_update(float delta_time) override;

    bool on_destroy() override;

    void load_model();
    void process_inputs(f32 deltaTime);

private:
    std::vector<hive::Vertex> vertices;
    std::vector<u32> indices;

    struct VikingRoom
    {
        hive::vk::VulkanBuffer vertex_buffer_{};
        hive::vk::VulkanBuffer index_buffer{};

        hive::vk::VulkanImage texture{};

    } viking_room{};


    hive::vk::VulkanBuffer ubo_buffer{};

    hive::vk::VulkanPipeline default_pipeline_{};

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};


bool BasicApp::on_init()
{
    // // ubo_handle = renderer_->createUbo();
    // // for (i32 i = 0; i < 3; ++i)
    // // {
    // //     handles[i] = renderer_->createShader("shaders/vert.spv", "shaders/frag.spv", ubo_handle, i);
    // // }
    //
    load_model();
    //
    // //Vertex buffer creation
    // {
    {
        hive::vk::VulkanBuffer temp_buffer{};

        u32 size  = sizeof(vertices[0]) * vertices.size();
        hive::vk::create_buffer(device_vulkan_->getDevice(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, size, temp_buffer);
        hive::vk::buffer_fill_data(device_vulkan_->getDevice(), temp_buffer, vertices.data(), size);

        hive::vk::create_buffer(device_vulkan_->getDevice(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size, viking_room.vertex_buffer_);
        hive::vk::buffer_copy(device_vulkan_->getDevice(), temp_buffer, viking_room.vertex_buffer_, size);
        hive::vk::destroy_buffer(device_vulkan_->getDevice(), temp_buffer);
    }

    //Index buffer
    {
        hive::vk::VulkanBuffer temp_buffer{};
        u32 size = sizeof(indices[0]) * indices.size();
        hive::vk::create_buffer(device_vulkan_->getDevice(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, size, temp_buffer);
        hive::vk::buffer_fill_data(device_vulkan_->getDevice(), temp_buffer, indices.data(), size);

        hive::vk::create_buffer(device_vulkan_->getDevice(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size, viking_room.index_buffer);
        hive::vk::buffer_copy(device_vulkan_->getDevice(), temp_buffer, viking_room.index_buffer, size);
        hive::vk::destroy_buffer(device_vulkan_->getDevice(), temp_buffer);
    }
    //Ubo buffer
    {
        hive::vk::create_buffer(device_vulkan_->getDevice(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(hive::UniformBufferObject), ubo_buffer);
    }
    //Texture image
    {
        i32 texWidth, texHeight, texChannels;
        stbi_uc *pixels = stbi_load("textures/viking_room.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        u32 image_size = texWidth * texHeight * 4;

        if (!pixels)
        {
            return false;
        }

        hive::vk::VulkanBuffer temp_buffer{};
        hive::vk::create_buffer(device_vulkan_->getDevice(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, image_size, temp_buffer);
        hive::vk::buffer_fill_data(device_vulkan_->getDevice(), temp_buffer, pixels, image_size);
        stbi_image_free(pixels);

        hive::vk::create_image(device_vulkan_->getDevice(), texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB,
                               VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, viking_room.texture);

        hive::vk::transition_image_layout(device_vulkan_->getDevice(), VK_FORMAT_R8G8B8A8_SRGB,
                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, viking_room.texture);
        hive::vk::copy_buffer_to_image(device_vulkan_->getDevice(), temp_buffer, viking_room.texture, texWidth,
                                       texHeight);
        hive::vk::transition_image_layout(device_vulkan_->getDevice(), VK_FORMAT_R8G8B8A8_SRGB,
                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, viking_room.texture);
        hive::vk::create_image_view(device_vulkan_->getDevice(), viking_room.texture.vk_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, viking_room.texture.vk_image_view);
        hive::vk::create_image_sampler(device_vulkan_->getDevice(), viking_room.texture.vk_sampler);

        hive::vk::destroy_buffer(device_vulkan_->getDevice(), temp_buffer);
    }
    //Pipeline
    {
        VkShaderModule vertex_module;
        hive::vk::create_shader_module(device_vulkan_->getDevice(), "shaders/vert.spv", vertex_module);

        VkShaderModule frag_module;
        hive::vk::create_shader_module(device_vulkan_->getDevice(), "shaders/frag.spv", frag_module);

        //TODO: add shader stage
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertex_module;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = frag_module;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device_vulkan_->getDevice().logical_device, &layoutInfo, nullptr,
                                        &default_pipeline_.descriptor_set_layout) != VK_SUCCESS)
        {
            return false;
        }

        hive::vk::create_graphics_pipeline(device_vulkan_->getDevice(), device_vulkan_->getRenderPass(), shaderStages, 2, 3, VK_POLYGON_MODE_FILL, default_pipeline_);


        hive::vk::destroy_shader_module(device_vulkan_->getDevice(), vertex_module);
        hive::vk::destroy_shader_module(device_vulkan_->getDevice(), frag_module);


        //TODO create descriptor pool and DescriptorSet
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = 3;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = 3;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 3;

        if (vkCreateDescriptorPool(device_vulkan_->getDevice().logical_device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            // throw std::runtime_error("failed to create descriptor pool!");
            return false;
        }

        std::vector<VkDescriptorSetLayout> layouts(3, default_pipeline_.descriptor_set_layout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 3;
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(3);
        if (vkAllocateDescriptorSets(device_vulkan_->getDevice().logical_device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
        {
            return false;
        }


        for(i32 i = 0; i < 3; i++) //MAX_FRAME_IN_FLIGHT
        {
            VkWriteDescriptorSet write_descriptor_set[2] = {};
            VkDescriptorBufferInfo info{};
            info.buffer = ubo_buffer.vk_buffer;
            info.offset = 0;
            info.range = sizeof(hive::UniformBufferObject);

            write_descriptor_set[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write_descriptor_set[0].dstSet = descriptorSets[i];
            write_descriptor_set[0].dstBinding = 0;
            write_descriptor_set[0].dstArrayElement = 0;
            write_descriptor_set[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            write_descriptor_set[0].descriptorCount = 1;
            write_descriptor_set[0].pBufferInfo = &info;
            write_descriptor_set[0].pImageInfo = nullptr; // Optional
            write_descriptor_set[0].pTexelBufferView = nullptr; // Optional

            VkDescriptorImageInfo info2{};
            info2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            info2.imageView = viking_room.texture.vk_image_view;
            //We have only 1 image view and sampler no matter how many frame in flight we have
            info2.sampler = viking_room.texture.vk_sampler;


            write_descriptor_set[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write_descriptor_set[1].dstSet = descriptorSets[i];
            write_descriptor_set[1].dstBinding = 1;
            write_descriptor_set[1].dstArrayElement = 0;
            write_descriptor_set[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            write_descriptor_set[1].descriptorCount = 1;
            write_descriptor_set[1].pBufferInfo = nullptr;
            write_descriptor_set[1].pImageInfo = &info2; // Optional
            write_descriptor_set[1].pTexelBufferView = nullptr; // Optional

            vkUpdateDescriptorSets(device_vulkan_->getDevice().logical_device, 2, write_descriptor_set, 0, nullptr);
        }
    }

    return true;
}

static auto startTime = std::chrono::high_resolution_clock::now();
void update_camera(hive::vk::GraphicsDevice_Vulkan &device_vulkan, hive::vk::VulkanBuffer &buffer)
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    hive::UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f),
                                1920 / (float) 1080, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(buffer.map, &ubo, sizeof(ubo));
}

bool BasicApp::on_update(float delta_time)
{
    process_inputs(delta_time);
    update_camera(*device_vulkan_, ubo_buffer);

    if (!device_vulkan_->BeginCmd()) return false;

    {


        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) device_vulkan_->getSwapchain().extent_2d.width;
        viewport.height = (float) device_vulkan_->getSwapchain().extent_2d.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(device_vulkan_->getCmdBuffer(), 0, 1, &viewport);


        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = device_vulkan_->getSwapchain().extent_2d;
        vkCmdSetScissor(device_vulkan_->getCmdBuffer(), 0, 1, &scissor);

        vkCmdBindPipeline(device_vulkan_->getCmdBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, default_pipeline_.vk_pipeline);
        vkCmdBindDescriptorSets(device_vulkan_->getCmdBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, default_pipeline_.pipeline_layout, 0, 1, &descriptorSets[0], 0, nullptr);

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(device_vulkan_->getCmdBuffer(), 0, 1, &viking_room.vertex_buffer_.vk_buffer, &offset);
        vkCmdBindIndexBuffer(device_vulkan_->getCmdBuffer(), viking_room.index_buffer.vk_buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(device_vulkan_->getCmdBuffer(), indices.size(), 1, 0, 0, 0);
    }
    if (!device_vulkan_->EndCmd()) return false;
    if (!device_vulkan_->SubmitFrame()) return false;
    return true;
}

bool BasicApp::on_destroy()
{
    device_vulkan_->WaitForGPU();

    hive::vk::destroy_buffer(device_vulkan_->getDevice(), ubo_buffer);
    hive::vk::destroy_buffer(device_vulkan_->getDevice(), viking_room.index_buffer);
    hive::vk::destroy_buffer(device_vulkan_->getDevice(), viking_room.vertex_buffer_);
    hive::vk::destroy_image(device_vulkan_->getDevice(), viking_room.texture);
    hive::vk::destroy_graphics_pipeline(device_vulkan_->getDevice(), default_pipeline_);
    hive::vk::destroy_descriptor_set_layout(device_vulkan_->getDevice(), default_pipeline_.descriptor_set_layout);
    hive::vk::destroy_descriptor_pool(device_vulkan_->getDevice(), descriptorPool);
    return true;
}

void BasicApp::load_model()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "assets/viking_room.obj")) {
        throw std::runtime_error(warn + err);
    }

    for(const auto& shape : shapes)
    {
        for(const auto& index : shape.mesh.indices)
        {
            hive::Vertex vertex{};
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};


            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }
}

void BasicApp::process_inputs(f32 delta_time)
{
	if (input_.isKeyPressed(hive::InputKey::Escape))
	{
        window_.unlockCursor();
	}
}

int main()
{
    hive::Logger logger;
    RegisterDefaultLoggerSync(hive::Logger::LogLevel::DEBUG);

    hive::ApplicationConfig config{};

    config.log_level = hive::Logger::LogLevel::DEBUG;
    config.window_config.width = 1080;
    config.window_config.height = 920;
    config.window_config.title = "Hive Engine";
    config.window_config.type = hive::WindowConfig::WindowType::GLFW;

    // hive::Application app(config);
    BasicApp app(config);
    app.run();


}
