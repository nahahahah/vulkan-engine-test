#include "VulkanHelpers/SubpassBeginInfo.hpp"

VkSubpassBeginInfo GenerateSubpassBeginInfo() {
    VkSubpassBeginInfo beginInfo {};

    // structure type
    beginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;

    // contents property
    beginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;
    
    // extend begin info
    beginInfo.pNext = VK_NULL_HANDLE;

    return beginInfo;
}
