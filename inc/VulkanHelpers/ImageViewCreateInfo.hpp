#ifndef VK_WRAPPER_IMAGE_VIEW_CREATE_INFO_CPP
#define VK_WRAPPER_IMAGE_VIEW_CREATE_INFO_CPP

#include <vulkan/vulkan.h>

VkImageViewCreateInfo GenerateImageViewCreateInfo(VkFormat format, VkImage const& image);

#endif // VK_WRAPPER_IMAGE_VIEW_CREATE_INFO_CPP
