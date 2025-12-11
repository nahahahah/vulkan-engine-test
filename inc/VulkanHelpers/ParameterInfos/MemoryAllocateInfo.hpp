#ifndef VK_WRAPPER_MEMORY_ALLOCATE_INFO_HPP
#define VK_WRAPPER_MEMORY_ALLOCATE_INFO_HPP

#include <vulkan/vulkan.h>

VkMemoryAllocateInfo GenerateMemoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex);

#endif // VK_WRAPPER_MEMORY_ALLOCATE_INFO_HPP