#include "VulkanHelpers/ParameterInfos/DeviceQueueInfo.hpp"

VkDeviceQueueInfo2 GenerateDeviceQueueInfo(uint32_t queueIndex, uint32_t queueFamilyIndex) {
    VkDeviceQueueInfo2 queueInfo {};

    // structure type and flags
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    queueInfo.flags = 0;

    // queue and queue family indices
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.queueIndex = queueIndex;

    // extend queue info
    queueInfo.pNext = VK_NULL_HANDLE;

    return queueInfo;
}
