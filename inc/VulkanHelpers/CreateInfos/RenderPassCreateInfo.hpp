#ifndef VK_WRAPPER_RENDER_PASS_CREATE_INFO_HPP
#define VK_WRAPPER_RENDER_PASS_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkRenderPassCreateInfo2 GenerateRenderPassCreateInfo(
    std::span<VkAttachmentDescription2> attachmentDescriptions,
    std::span<VkSubpassDependency2> subpassDependencies,
    std::span<VkSubpassDescription2> subpassDescriptions
);

#endif // VK_WRAPPER_RENDER_PASS_CREATE_INFO_HPP
