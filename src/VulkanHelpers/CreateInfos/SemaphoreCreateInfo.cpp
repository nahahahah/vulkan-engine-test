#include "VulkanHelpers/CreateInfos/SemaphoreCreateInfo.hpp"

VkSemaphoreCreateInfo GenerateSemaphoreCreateInfo(uint32_t flags) {
    VkSemaphoreCreateInfo createInfo {};
    
    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.flags = flags;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
