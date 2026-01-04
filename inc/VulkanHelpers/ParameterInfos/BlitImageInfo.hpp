#ifndef VK_WRAPPER_BLIT_IMAGE_INFO_HPP
#define VK_WRAPPER_BLIT_IMAGE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Image.hpp"

VkBlitImageInfo2 GenerateBlitImageInfo(
    Image const& srcImage,
    VkImageLayout srcImageLayout,
    Image const& dstImage,
    VkImageLayout dstImageLayout,
    std::span<VkImageBlit2> regions,
    VkFilter filter
);

#endif // VK_WRAPPER_BLIT_IMAGE_INFO_HPP
