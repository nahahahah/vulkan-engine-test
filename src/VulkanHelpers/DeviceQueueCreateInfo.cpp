#include "VulkanHelpers/DeviceQueueCreateInfo.hpp"

VkDeviceQueueCreateInfo GenerateDeviceQueueCreateInfo(uint32_t queueFamily, float* queuePriority) {
    VkDeviceQueueCreateInfo createInfo {};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    createInfo.pQueuePriorities = queuePriority;
    createInfo.queueCount = 1;
    createInfo.queueFamilyIndex = queueFamily;

    createInfo.flags = 0;
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
