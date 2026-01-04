#include "VulkanHelpers/ParameterInfos/BufferMemoryRequirementsInfo.hpp"

VkBufferMemoryRequirementsInfo2 GenerateBufferMemoryRequirementsInfo(Buffer const& buffer) {
    VkBufferMemoryRequirementsInfo2 info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
    
    // buffer
    info.buffer = buffer.Handle();

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
