#ifndef VK_WRAPPER_IMAGE_MEMORY_REQUIREMENTS_INFO_HPP
#define VK_WRAPPER_IMAGE_MEMORY_REQUIREMENTS_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Image.hpp"

VkImageMemoryRequirementsInfo2 GenerateImageMemoryRequirementsInfo(Image const& image);

#endif // VK_WRAPPER_IMAGE_MEMORY_REQUIREMENTS_INFO_HPP
