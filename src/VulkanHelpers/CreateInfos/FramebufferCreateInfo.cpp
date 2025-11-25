#include "VulkanHelpers/CreateInfos/FramebufferCreateInfo.hpp"

VkFramebufferCreateInfo GenerateFramebufferCreateInfo(
    VkExtent2D extent,
    std::span<VkImageView> attachments,
    RenderPass const& renderPass
) {
    VkFramebufferCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.flags = 0;

    // extent
    createInfo.width = extent.width;
    createInfo.height = extent.height;

    // attachments
    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();

    // framebuffer properties
    createInfo.layers = 1;
    createInfo.renderPass = renderPass.Handle();

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;
    
    return createInfo;
}
