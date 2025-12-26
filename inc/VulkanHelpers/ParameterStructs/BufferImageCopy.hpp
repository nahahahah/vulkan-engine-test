#ifndef VK_WRAPPER_BUFFER_IMAGE_COPY_HPP
#define VK_WRAPPER_BUFFER_IMAGE_COPY_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Buffer.hpp"
#include "VulkanHelpers/Handles/Image.hpp"

VkBufferImageCopy2 GenerateBufferImageCopy(
    uint32_t width,
    uint32_t height
);

#endif // VK_WRAPPER_BUFFER_IMAGE_COPY_HPP
