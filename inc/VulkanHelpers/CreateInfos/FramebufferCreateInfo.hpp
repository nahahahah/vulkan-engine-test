#ifndef VK_WRAPPER_FRAMEBUFFER_CREATE_INFO_HPP
#define VK_WRAPPER_FRAMEBUFFER_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/RenderPass.hpp"

VkFramebufferCreateInfo GenerateFramebufferCreateInfo(
    VkExtent2D extent,
    std::span<VkImageView> attachments,
    RenderPass const& renderPass
);

#endif // VK_WRAPPER_FRAMEBUFFER_CREATE_INFO_HPP
