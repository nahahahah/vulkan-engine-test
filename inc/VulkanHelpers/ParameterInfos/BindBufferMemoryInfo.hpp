#ifndef VK_WRAPPER_BIND_BUFFER_MEMORY_INFO_HPP
#define VK_WRAPPER_BIND_BUFFER_MEMORY_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/DeviceMemory.hpp"
#include "VulkanHelpers/Handles/Buffer.hpp"

VkBindBufferMemoryInfo GenerateBindBufferMemoryInfo(DeviceMemory const& memory, VkDeviceSize memoryOffset, Buffer const& buffer);

#endif // VK_WRAPPER_BIND_BUFFER_MEMORY_INFO_HPP