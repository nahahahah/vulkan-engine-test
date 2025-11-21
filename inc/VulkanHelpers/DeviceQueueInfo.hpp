#ifndef VK_WRAPPER_DEVICE_QUEUE_INFO_HPP
#define VK_WRAPPER_DEVICE_QUEUE_INFO_HPP

#include <vulkan/vulkan.h>

VkDeviceQueueInfo2 GenerateDeviceQueueInfo(uint32_t queueIndex, uint32_t queueFamilyIndex);

#endif // VK_WRAPPER_DEVICE_QUEUE_INFO_HPP
