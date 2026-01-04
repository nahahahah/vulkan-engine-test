#ifndef VK_WRAPPER_IMAGE_CREATE_INFO_HPP
#define VK_WRAPPER_IMAGE_CREATE_INFO_HPP

#include <vulkan/vulkan.h>

VkImageCreateInfo GenerateImageCreateInfo(
    VkExtent3D const& dimensions,
    VkImageUsageFlags usage,
    VkSharingMode sharingMode,
    VkFormat format,
    VkImageTiling tiling
);

#endif // VK_WRAPPER_IMAGE_CREATE_INFO_HPP
