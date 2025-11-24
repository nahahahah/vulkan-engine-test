#ifndef VK_WRAPPER_COMMAND_BUFFER_SUBMIT_INFO_HPP
#define VK_WRAPPER_COMMAND_BUFFER_SUBMIT_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/CommandBuffer.hpp"

VkCommandBufferSubmitInfo GenerateCommandBufferSubmitInfo(CommandBuffer const& commandBuffer);

#endif // VK_WRAPPER_COMMAND_BUFFER_SUBMIT_INFO_HPP
