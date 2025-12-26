#include "VulkanHelpers/ParameterInfos/BindBufferMemoryInfo.hpp"

VkBindBufferMemoryInfo GenerateBindBufferMemoryInfo(Buffer const& buffer, DeviceMemory const& memory, VkDeviceSize memoryOffset) {
    VkBindBufferMemoryInfo info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO;

    // bind buffer properties
    info.buffer = buffer.Handle();
    info.memory = memory.Handle();
    info.memoryOffset = memoryOffset;

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
