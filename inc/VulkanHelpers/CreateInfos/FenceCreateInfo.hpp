#ifndef VK_WRAPPER_FENCE_CREATE_INFO_HPP
#define VK_WRAPPER_FENCE_CREATE_INFO_HPP

#include <vulkan/vulkan.h>

VkFenceCreateInfo GenerateFenceCreateInfo(uint32_t flags = 0);

#endif // VK_WRAPPER_FENCE_CREATE_INFO_HPP
