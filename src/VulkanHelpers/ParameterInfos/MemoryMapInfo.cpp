#include "VulkanHelpers/ParameterInfos/MemoryMapInfo.hpp"

VkMemoryMapInfo GenerateMemoryMapInfo(DeviceMemory const& memory, VkDeviceSize size, VkDeviceSize offset) {
    VkMemoryMapInfo info {};

    // structure type and flags
    info.sType = VK_STRUCTURE_TYPE_MEMORY_MAP_INFO;
    info.flags = 0;

    // map properties
    info.memory = memory.Handle();
    info.size = size;
    info.offset = offset;

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
