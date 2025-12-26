#ifndef VK_WRAPPER_IMAGE_MEMORY_BARRIER_HPP
#define VK_WRAPPER_IMAGE_MEMORY_BARRIER_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Image.hpp"

VkImageMemoryBarrier2 GenerateImageMemoryBarrier(
    Image const& image,
    VkImageLayout oldLayout,
    VkImageLayout newLayout
);

#endif // VK_WRAPPER_IMAGE_MEMORY_BARRIER_HPP
