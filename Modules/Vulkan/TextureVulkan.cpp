#include "TextureVulkan.h"
#include "Graphic/GFX.h"
#include "GraphicDeviceVulkan.h"
#include "BufferVulkan.h"
#include "CommandVulkan.h"
#include "UtilsVulkan.h"

void copyBufferToImage(const hive::gfx::DeviceHandle* device, hive::gfx::CommandPoolHandle *command_pool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = hive::vk::beginSingleTimeCommands(device, command_pool);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    hive::vk::endSingleTimeCommands(device, command_pool, commandBuffer);
}

void transitionImageLayout(const hive::gfx::DeviceHandle *device, hive::gfx::CommandPoolHandle *command_pool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = hive::vk::beginSingleTimeCommands(device, command_pool);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    hive::vk::endSingleTimeCommands(device, command_pool, commandBuffer);
}


VkFormat findSupportedFormat(const hive::gfx::DeviceHandle* device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(device->m_vkb_phys_device, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat get_vulkan_format(const hive::gfx::TextureFormat &format)
{
    switch (format)
    {
    case hive::gfx::TextureFormat::TEXTURE_FORMAT_D32_SFLOAT:
        return VK_FORMAT_D32_SFLOAT;

    case hive::gfx::TextureFormat::TEXTURE_FORMAT_B8G8R8A8_UNORM:
        return VK_FORMAT_B8G8R8A8_UNORM;

    case hive::gfx::TextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM:
        return VK_FORMAT_R8G8B8A8_UNORM;

    case hive::gfx::TextureFormat::TEXTURE_FORMAT_R32G32B32_SFLOAT:
        return VK_FORMAT_R32G32B32_SFLOAT;

    case hive::gfx::TextureFormat::TEXTURE_FORMAT_R32G32_SFLOAT:
        return VK_FORMAT_R32G32_SFLOAT;

    case hive::gfx::TextureFormat::TEXTURE_FORMAT_R8_UNORM:
        return VK_FORMAT_R8_UNORM;

    case hive::gfx::TextureFormat::TEXTURE_FORMAT_R8G8B8A8_SRGB:
        return VK_FORMAT_R8G8B8A8_SRGB;

    default:
        HIVE_ASSERT(false, "We should never end here");
        return VK_FORMAT_UNDEFINED;
    }
}


VkFormat findDepthFormat(const hive::gfx::DeviceHandle *device) {
    return findSupportedFormat(device,
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkImageTiling get_image_tiling(const hive::gfx::TextureTiling option)
{
    switch (option)
    {
        case hive::gfx::TEXTURE_TILING_OPTIMAL:
            return VK_IMAGE_TILING_OPTIMAL;
        case hive::gfx::TEXTURE_TILING_LINEAR:
            return VK_IMAGE_TILING_LINEAR;
    }

    return VK_IMAGE_TILING_OPTIMAL;
}

VkImageAspectFlags get_image_aspect(const uint32 aspect)
{
    VkImageAspectFlags aspect_flags = 0;
    if (aspect & hive::gfx::TextureAspectFlagBits::TEXTURE_ASPECT_COLOR)
    {
        aspect_flags |= VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (aspect & hive::gfx::TextureAspectFlagBits::TEXTURE_ASPECT_DEPTH)
    {
        aspect_flags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    }

    if (aspect & hive::gfx::TextureAspectFlagBits::TEXTURE_ASPECT_STENCIL)
    {
        aspect_flags |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    return aspect_flags;
}



VkImageUsageFlags get_vulkan_texture_usage(const uint32_t texture_usage)
{
    VkImageUsageFlags usage = 0;

    if (texture_usage & hive::gfx::TEXTURE_USAGE_SAMPLE)
        usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

    if (texture_usage & hive::gfx::TEXTURE_USAGE_DEPTH)
        usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    if (texture_usage & hive::gfx::TEXTURE_USAGE_COLOR)
        usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (texture_usage & hive::gfx::TEXTURE_USAGE_SRC)
        usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    if (texture_usage & hive::gfx::TEXTURE_USAGE_DST)
        usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    if (texture_usage & hive::gfx::TEXTURE_USAGE_STORAGE)
        usage |= VK_IMAGE_USAGE_STORAGE_BIT;

    return usage;
}


hive::gfx::TextureHandle * hive::vk::vulkan_texture_create(const hive::gfx::DeviceHandle *device,
                                                           const hive::gfx::TextureDesc &desc)
{
    //Image
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = desc.width;
    imageInfo.extent.height = desc.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.tiling = get_image_tiling(desc.tiling);
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    //TODO convert those from desc to
    imageInfo.format = get_vulkan_format(desc.format);
    // imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.usage = get_vulkan_texture_usage(desc.usage_flags);

    VkImage image = VK_NULL_HANDLE;
    if (vkCreateImage(device->m_vkb_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        HIVE_LOG_ERROR("Failed to create vulkan image");
        return nullptr;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device->m_vkb_device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(device, memRequirements.memoryTypeBits, desc.memory_property_flags);

    VkDeviceMemory imageMemory = VK_NULL_HANDLE;
    if (vkAllocateMemory(device->m_vkb_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        HIVE_LOG_ERROR("Failed to allocate vulkan image memory!");
    }

    vkBindImageMemory(device->m_vkb_device, image, imageMemory, 0);

    gfx::TextureHandle *handle = static_cast<gfx::TextureHandle *>(HV_NEW(sizeof(gfx::TextureHandle), HIVE_ALLOC_CAT_GRAPHIC));

    handle->image = image;
    handle->image_info = imageInfo;
    handle->memory = imageMemory;

    //Image view
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    viewInfo.format = get_vulkan_format(desc.format);
    //TODO change this so it's from the TextureDesc
    viewInfo.subresourceRange.aspectMask = get_image_aspect(desc.aspect_flags);


    VkImageView imageView;
    if (vkCreateImageView(device->m_vkb_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    //Sampler
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(device->m_vkb_phys_device, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    VkSampler sampler = VK_NULL_HANDLE;
    if (desc.usage_flags & gfx::TEXTURE_USAGE_SAMPLE) //We only create a sampler if we need it
    {
        if (vkCreateSampler(device->m_vkb_device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
        {
            HIVE_LOG_ERROR("Failed to create vulkan sampler");
            return nullptr;
        }
    }

    handle->image_view = imageView;
    handle->image_view_info = viewInfo;
    handle->sampler = sampler;

    return handle;
}

void hive::vk::vulkan_texture_update(const hive::gfx::DeviceHandle *device, hive::gfx::CommandPoolHandle *command_pool, hive::gfx::TextureHandle *texture,
    const void *data, size_t size)
{

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

    createBuffer(device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* img_data;
    vkMapMemory(device->m_vkb_device, stagingBufferMemory, 0, size, 0, &img_data);
    memcpy(img_data, data, static_cast<size_t>(size));
    vkUnmapMemory(device->m_vkb_device, stagingBufferMemory);

    // stbi_image_free(pixels);

    // createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

    transitionImageLayout(device, command_pool, texture->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(device, command_pool, stagingBuffer, texture->image, static_cast<uint32_t>(texture->image_info.extent.width), static_cast<uint32_t>(texture->image_info.extent.height));
    transitionImageLayout(device, command_pool, texture->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device->m_vkb_device, stagingBuffer, nullptr);
    vkFreeMemory(device->m_vkb_device, stagingBufferMemory, nullptr);
}

void hive::vk::vulkan_texture_destroy(const hive::gfx::DeviceHandle *device, hive::gfx::TextureHandle *texture)
{
    vkDestroySampler(device->m_vkb_device, texture->sampler, nullptr);
    vkDestroyImageView(device->m_vkb_device, texture->image_view, nullptr);
    vkDestroyImage(device->m_vkb_device, texture->image, nullptr);
    vkFreeMemory(device->m_vkb_device, texture->memory, nullptr);
    HV_FREE(texture);
}

hive::gfx::TextureHandle * hive::vk::vulkan_texture_copy(const hive::gfx::DeviceHandle *device,
    hive::gfx::TextureHandle *texture)
{
    return nullptr;
}
