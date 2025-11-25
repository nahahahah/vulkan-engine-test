#include "VulkanHelpers/CreateInfos/DeviceCreateInfo.hpp"

VkDeviceCreateInfo GenerateDeviceCreateInfo(
    std::span<VkDeviceQueueCreateInfo> queueCreateInfos,
    std::span<char const*> extensions,
    std::span<char const*> layers,
    VkPhysicalDeviceFeatures2 const& features,
    uint32_t flags,
    void* next
) {
    VkDeviceCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.flags = flags;

    // device extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    // device layers
    createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    createInfo.ppEnabledLayerNames = layers.data();

    // device features
    createInfo.pEnabledFeatures = &features.features;

    // device queue create infos
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    // extend create info
    createInfo.pNext = next;

    return createInfo;
}
