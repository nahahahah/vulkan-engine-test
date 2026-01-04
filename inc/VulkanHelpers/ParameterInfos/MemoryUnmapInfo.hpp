#ifndef VK_WRAPPER_MEMORY_UNMAP_INFO_HPP
#define VK_WRAPPER_MEMORY_UNMAP_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/DeviceMemory.hpp"

VkMemoryUnmapInfo GenerateMemoryUnmapInfo(DeviceMemory const& memory);

#endif // VK_WRAPPER_MEMORY_UNMAP_INFO_HPP
