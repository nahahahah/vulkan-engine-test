#ifndef VK_WRAPPER_DEVICE_QUEUE_CREATE_INFO_HPP
#define VK_WRAPPER_DEVICE_QUEUE_CREATE_INFO_HPP

#include <vulkan/vulkan.h>

VkDeviceQueueCreateInfo GenerateDeviceQueueCreateInfo(uint32_t queueFamily, float* queuePriority);

#endif // VK_WRAPPER_DEVICE_QUEUE_CREATE_INFO_HPP
