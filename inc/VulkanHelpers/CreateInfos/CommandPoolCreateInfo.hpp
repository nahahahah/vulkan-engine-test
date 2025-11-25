#ifndef VK_WRAPPER_COMMAND_POOL_CREATE_INFO_HPP
#define VK_WRAPPER_COMMAND_POOL_CREATE_INFO_HPP

#include <vulkan/vulkan.h>

VkCommandPoolCreateInfo GenerateCommandPoolCreateInfo(uint32_t queueFamilyIndex);

#endif // VK_WRAPPER_COMMAND_POOL_CREATE_INFO_HPP
