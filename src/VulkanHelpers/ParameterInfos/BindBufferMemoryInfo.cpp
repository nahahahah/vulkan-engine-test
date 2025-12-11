#include "VulkanHelpers/ParameterInfos/BindBufferMemoryInfo.hpp"

VkBindBufferMemoryInfo GenerateBindBufferMemoryInfo(DeviceMemory const& memory, VkDeviceSize memoryOffset, Buffer const& buffer) {
    VkBindBufferMemoryInfo info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO;

    // bind buffer properties
    info.memory = memory.Handle();
    info.memoryOffset = memoryOffset;
    info.buffer = buffer.Handle();

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
