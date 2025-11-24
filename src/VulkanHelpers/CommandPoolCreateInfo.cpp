#include "VulkanHelpers/CommandPoolCreateInfo.hpp"

VkCommandPoolCreateInfo GenerateCommandPoolCreateInfo(uint32_t queueFamilyIndex) {
    VkCommandPoolCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    // queue family index
    createInfo.queueFamilyIndex = queueFamilyIndex;

    // extend create ino
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
