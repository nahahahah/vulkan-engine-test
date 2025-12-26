#include "VulkanHelpers/ParameterStructs/BufferImageCopy.hpp"

VkBufferImageCopy2 GenerateBufferImageCopy(
    uint32_t width,
    uint32_t height
) {
    VkBufferImageCopy2 bufferImageCopy {};

    // structure type
    bufferImageCopy.sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2;

    // buffer properties
    bufferImageCopy.bufferOffset = 0;
    bufferImageCopy.bufferRowLength = 0;
    bufferImageCopy.bufferImageHeight = 0;

    // image properties
    bufferImageCopy.imageOffset = { 0, 0, 0 };
    bufferImageCopy.imageExtent = { width, height, 1 };

    // image subresources properties
    bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferImageCopy.imageSubresource.mipLevel = 0;
    bufferImageCopy.imageSubresource.baseArrayLayer = 0;
    bufferImageCopy.imageSubresource.layerCount = 1;
    
    // extend buffer image copy
    bufferImageCopy.pNext = VK_NULL_HANDLE;

    return bufferImageCopy;
}
