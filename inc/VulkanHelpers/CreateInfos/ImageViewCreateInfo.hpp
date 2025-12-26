#ifndef VK_WRAPPER_IMAGE_VIEW_CREATE_INFO_CPP
#define VK_WRAPPER_IMAGE_VIEW_CREATE_INFO_CPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Image.hpp"

VkImageViewCreateInfo GenerateImageViewCreateInfo(VkFormat format, Image const& image);

#endif // VK_WRAPPER_IMAGE_VIEW_CREATE_INFO_CPP
