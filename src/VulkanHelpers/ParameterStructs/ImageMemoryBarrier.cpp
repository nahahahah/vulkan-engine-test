#include "VulkanHelpers/ParameterStructs/ImageMemoryBarrier.hpp"

VkImageMemoryBarrier2 GenerateImageMemoryBarrier(
    Image const& image,
    VkImageLayout oldLayout,
    VkImageLayout newLayout
) {
    VkImageMemoryBarrier2 imageMemoryBarrier {};

    // structure type
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;

    // image properties
    imageMemoryBarrier.image = image.Handle(); 

    // layout properties
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.newLayout = newLayout;

    // queue family indices properties
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    // stage masks properties
    imageMemoryBarrier.srcStageMask = 0;
    imageMemoryBarrier.dstStageMask = 0;

    // access masks properties
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = 0;

    // subresource range properties
    imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    imageMemoryBarrier.subresourceRange.layerCount = 1;
    imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    imageMemoryBarrier.subresourceRange.levelCount = 1;

    // extend image memory barrier
    imageMemoryBarrier.pNext = VK_NULL_HANDLE;

    return imageMemoryBarrier;
}
