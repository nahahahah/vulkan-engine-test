#include "VulkanHelpers/SubpassDescription.hpp"

VkSubpassDescription2 GenerateSubpassDescription(std::span<VkAttachmentReference2> colorAttachments) {
    VkSubpassDescription2 description {};

    // structure type and flags
    description.sType = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2;
    description.flags = 0;

    // colors attachments
    description.colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
    description.pColorAttachments = colorAttachments.data();

    // input attachments
    description.inputAttachmentCount = 0;
    description.pInputAttachments = VK_NULL_HANDLE;

    // depth stencil attachment
    description.pDepthStencilAttachment = VK_NULL_HANDLE;

    // pipeline bindpoint
    description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    
    // preserve attachments
    description.preserveAttachmentCount = 0;
    description.pPreserveAttachments = VK_NULL_HANDLE;

    // subpass properties
    description.pResolveAttachments = VK_NULL_HANDLE;
    description.viewMask = 0;

    // extend description
    description.pNext = VK_NULL_HANDLE;

    return description;
}