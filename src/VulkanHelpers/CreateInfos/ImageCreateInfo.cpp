#include "VulkanHelpers/CreateInfos/ImageCreateInfo.hpp"

VkImageCreateInfo GenerateImageCreateInfo(
    VkExtent3D const& dimensions,
    uint32_t mipLevels,
    VkImageUsageFlags usage,
    VkSharingMode sharingMode,
    VkFormat format,
    VkImageTiling tiling
) {
    VkImageCreateInfo createInfo {};

    if (mipLevels < 1) {
        mipLevels = 1;
    }
    
    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.flags = 0;

    // image properties
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.usage = usage;
    createInfo.sharingMode = sharingMode;
    createInfo.extent = dimensions;
    createInfo.format = format;
    createInfo.tiling = tiling;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    
    // queue family indices properties
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices;

    // array properties
    createInfo.arrayLayers = 1;
    
    // mips and multisamples properties
    createInfo.mipLevels = mipLevels;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
