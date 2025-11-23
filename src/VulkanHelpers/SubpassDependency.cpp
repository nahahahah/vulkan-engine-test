#include "VulkanHelpers/SubpassDependency.hpp"

VkSubpassDependency2 GenerateSubpassDependency() {
    VkSubpassDependency2 dependency {};

    // structure type
    dependency.sType = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2;

    // stage mask
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    // access mask
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // subpass
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;

    // dependency properties
    dependency.viewOffset = 0;
    dependency.dependencyFlags = 0;

    // extend dependency
    dependency.pNext = VK_NULL_HANDLE;

    return dependency;
}
