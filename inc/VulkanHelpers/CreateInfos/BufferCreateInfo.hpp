#ifndef VK_WRAPPER_BUFFER_CREATE_INFO_HPP
#define VK_WRAPPER_BUFFER_CREATE_INFO_HPP

#include <vulkan/vulkan.h>

VkBufferCreateInfo GenerateBufferCreateInfo(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode
);

#endif // VK_WRAPPER_BUFFER_CREATE_INFO_HPP