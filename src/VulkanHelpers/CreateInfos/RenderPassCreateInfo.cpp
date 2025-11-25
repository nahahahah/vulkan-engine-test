#include "VulkanHelpers/CreateInfos/RenderPassCreateInfo.hpp"

VkRenderPassCreateInfo2 GenerateRenderPassCreateInfo(
    std::span<VkAttachmentDescription2> attachmentDescriptions,
    std::span<VkSubpassDependency2> subpassDependencies,
    std::span<VkSubpassDescription2> subpassDescriptions
) {
    VkRenderPassCreateInfo2 createInfo {};

    // structure info and flags
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2;
    createInfo.flags = 0;

    // attachments
    createInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
    createInfo.pAttachments = attachmentDescriptions.data();

    // dependencies
    createInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
    createInfo.pDependencies = subpassDependencies.data();

    // subpasses
    createInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
    createInfo.pSubpasses = subpassDescriptions.data();
    
    // correlated view masks
    createInfo.correlatedViewMaskCount = 0;
    createInfo.pCorrelatedViewMasks = VK_NULL_HANDLE;
    
    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;
    
    return createInfo;
}
