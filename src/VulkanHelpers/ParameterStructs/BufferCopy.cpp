#include "VulkanHelpers/ParameterStructs/BufferCopy.hpp"

VkBufferCopy2 GenerateBufferCopy(VkDeviceSize size) {
    VkBufferCopy2 bufferCopy {};

    // structure type
    bufferCopy.sType = VK_STRUCTURE_TYPE_BUFFER_COPY_2;

    // buffer copy properties
    bufferCopy.size = size;
    bufferCopy.srcOffset = 0;
    bufferCopy.dstOffset = 0;

    // extend buffer copy
    bufferCopy.pNext = VK_NULL_HANDLE;

    return bufferCopy;
}
