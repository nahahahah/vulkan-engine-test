#include "VulkanHelpers/CreateInfos/FenceCreateInfo.hpp"

VkFenceCreateInfo GenerateFenceCreateInfo(uint32_t flags) {
    VkFenceCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = flags;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
