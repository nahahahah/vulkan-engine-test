#include "VulkanHelpers/CreateInfos/BufferCreateInfo.hpp"

VkBufferCreateInfo GenerateBufferCreateInfo(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode
) {
    VkBufferCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.flags = 0;
    
    // queue family indices
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = VK_NULL_HANDLE;

    // buffer specific properties
    createInfo.size = size;
    createInfo.usage = usage;
    createInfo.sharingMode = sharingMode;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
