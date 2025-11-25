#include "VulkanHelpers/CreateInfos/DeviceQueueCreateInfo.hpp"

VkDeviceQueueCreateInfo GenerateDeviceQueueCreateInfo(uint32_t queueFamily, float* queuePriority) {
    VkDeviceQueueCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    createInfo.flags = 0;

    // queue properties
    createInfo.pQueuePriorities = queuePriority;
    createInfo.queueCount = 1;
    createInfo.queueFamilyIndex = queueFamily;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
