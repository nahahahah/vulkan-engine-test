#ifndef VK_WRAPPER_RENDER_PASS_BEGIN_INFO_HPP
#define VK_WRAPPER_RENDER_PASS_BEGIN_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/RenderPass.hpp"
#include "VulkanHelpers/Framebuffer.hpp"
#include "VulkanHelpers/ClearValue.hpp"
#include "VulkanHelpers/Rect2D.hpp"

VkRenderPassBeginInfo GenerateRenderPassBeginInfo(
    RenderPass const& renderPass,
    Framebuffer const& framebuffer,
    std::span<VkClearValue> clearValues,
    VkRect2D renderArea,
    void* next = VK_NULL_HANDLE
);

#endif // VK_WRAPPER_RENDER_PASS_BEGIN_INFO_HPP
