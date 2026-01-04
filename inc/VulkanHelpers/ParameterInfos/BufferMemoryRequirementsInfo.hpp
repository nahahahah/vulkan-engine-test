#ifndef VK_WRAPPER_BUFFER_MEMORY_REQUIREMENTS_INFO_HPP
#define VK_WRAPPER_BUFFER_MEMORY_REQUIREMENTS_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Buffer.hpp"

VkBufferMemoryRequirementsInfo2 GenerateBufferMemoryRequirementsInfo(Buffer const& buffer);

#endif // VK_WRAPPER_BUFFER_MEMORY_REQUIREMENTS_INFO_HPP
