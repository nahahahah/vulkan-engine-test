#ifndef VK_WRAPPER_IMAGE_MEMORY_BARRIER_HPP
#define VK_WRAPPER_IMAGE_MEMORY_BARRIER_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Image.hpp"

VkImageMemoryBarrier2 GenerateImageMemoryBarrier(
    Image const& image,
    VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    VkImageLayout newLayout = VK_IMAGE_LAYOUT_UNDEFINED
);

#endif // VK_WRAPPER_IMAGE_MEMORY_BARRIER_HPP
