#ifndef VK_WRAPPER_MEMORY_MAP_INFO_HPP
#define VK_WRAPPER_MEMORY_MAP_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/DeviceMemory.hpp"

VkMemoryMapInfo GenerateMemoryMapInfo(DeviceMemory const& memory, VkDeviceSize size, VkDeviceSize offset);

#endif // VK_WRAPPER_MEMORY_MAP_INFO_HPP
