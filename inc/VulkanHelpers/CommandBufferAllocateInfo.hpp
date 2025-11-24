#ifndef VK_WRAPPER_COMMAND_BUFFER_ALLOCATE_INFO_HPP
#define VK_WRAPPER_COMMAND_BUFFER_ALLOCATE_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/CommandPool.hpp"

VkCommandBufferAllocateInfo GenerateCommandBufferAllocateInfo(CommandPool const& commandPool, uint32_t count);

#endif // VK_WRAPPER_COMMAND_BUFFER_ALLOCATE_INFO_HPP
