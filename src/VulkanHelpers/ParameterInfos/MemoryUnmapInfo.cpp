#include "VulkanHelpers/ParameterInfos/MemoryUnmapInfo.hpp"

VkMemoryUnmapInfo GenerateMemoryUnmapInfo(DeviceMemory const& memory) {
    VkMemoryUnmapInfo info {};

    // structure type and flags
    info.sType = VK_STRUCTURE_TYPE_MEMORY_UNMAP_INFO;
    info.flags = 0;

    // unmap properties
    info.memory = memory.Handle();

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
