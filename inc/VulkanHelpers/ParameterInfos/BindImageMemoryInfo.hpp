#ifndef VK_WRAPPER_BIND_IMAGE_MEMORY_INFO_HPP
#define VK_WRAPPER_BIND_IMAGE_MEMORY_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Image.hpp"
#include "VulkanHelpers/Handles/DeviceMemory.hpp"

VkBindImageMemoryInfo GenerateBindImageMemoryInfo(Image const& image, DeviceMemory const& memory, VkDeviceSize memoryOffset);

#endif // VK_WRAPPER_BIND_IMAGE_MEMORY_INFO_HPP
