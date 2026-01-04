#ifndef VK_WRAPPER_BIND_BUFFER_MEMORY_INFO_HPP
#define VK_WRAPPER_BIND_BUFFER_MEMORY_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Buffer.hpp"
#include "VulkanHelpers/Handles/DeviceMemory.hpp"

VkBindBufferMemoryInfo GenerateBindBufferMemoryInfo(Buffer const& buffer, DeviceMemory const& memory, VkDeviceSize memoryOffset);

#endif // VK_WRAPPER_BIND_BUFFER_MEMORY_INFO_HPP