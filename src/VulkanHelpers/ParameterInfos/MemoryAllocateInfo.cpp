#include "VulkanHelpers/ParameterInfos/MemoryAllocateInfo.hpp"

VkMemoryAllocateInfo GenerateMemoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex) {
    VkMemoryAllocateInfo info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    // memory allocate properties
    info.allocationSize = allocationSize;
    info.memoryTypeIndex = memoryTypeIndex;

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
