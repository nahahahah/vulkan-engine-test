#ifndef VK_WRAPPER_SUBPASS_DESCRIPTION_HPP
#define VK_WRAPPER_SUBPASS_DESCRIPTION_HPP

#include <span>

#include <vulkan/vulkan.h>

VkSubpassDescription2 GenerateSubpassDescription(std::span<VkAttachmentReference2> colorAttachments);

#endif // VK_WRAPPER_SUBPASS_DESCRIPTION_HPP
