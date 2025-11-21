#ifndef VK_WRAPPER_DEVICE_CREATE_INFO_HPP
#define VK_WRAPPER_DEVICE_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkDeviceCreateInfo GenerateDeviceCreateInfo(
    std::span<VkDeviceQueueCreateInfo> queueCreateInfos,
    std::span<char const*> extensions = {},
    std::span<char const*> layers = {},
    VkPhysicalDeviceFeatures2 const& features = {},
    uint32_t flags = 0,
    void* next = VK_NULL_HANDLE
);

#endif // VK_WRAPPER_DEVICE_CREATE_INFO_HPP
