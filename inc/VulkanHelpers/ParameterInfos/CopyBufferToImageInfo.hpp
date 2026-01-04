#ifndef VK_WRAPPER_COPY_BUFFER_TO_IMAGE_INFO_HPP
#define VK_WRAPPER_COPY_BUFFER_TO_IMAGE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Buffer.hpp"
#include "VulkanHelpers/Handles/Image.hpp"

VkCopyBufferToImageInfo2 GenerateCopyBufferToImageInfo(
    Buffer const& buffer,
    Image const& image,
    VkImageLayout dstImageLayout,
    std::span<VkBufferImageCopy2> regions
);

#endif // VK_WRAPPER_COPY_BUFFER_TO_IMAGE_INFO_HPP
