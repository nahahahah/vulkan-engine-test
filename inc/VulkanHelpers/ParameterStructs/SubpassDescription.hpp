#ifndef VK_WRAPPER_SUBPASS_DESCRIPTION_HPP
#define VK_WRAPPER_SUBPASS_DESCRIPTION_HPP

#include <span>

#include <vulkan/vulkan.h>

VkSubpassDescription2 GenerateSubpassDescription(
    std::span<VkAttachmentReference2> colorAttachments,
    std::span<VkAttachmentReference2> resolveAttachments,
    VkAttachmentReference2 const* depthAttachment = VK_NULL_HANDLE
);

#endif // VK_WRAPPER_SUBPASS_DESCRIPTION_HPP
