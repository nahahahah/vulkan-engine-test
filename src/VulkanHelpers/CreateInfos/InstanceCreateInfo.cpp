#include "VulkanHelpers/CreateInfos/InstanceCreateInfo.hpp"

VkInstanceCreateInfo GenerateInstanceCreateInfo(
    VkApplicationInfo*     applicationInfo,
    std::span<char const*> enabledExtensionNames,
    std::span<char const*> enabledLayerNames,
    uint32_t               flags,
    void*                  next
) {
    VkInstanceCreateInfo createInfo {};

    // structure type and flags    
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = flags;

    // application info
    createInfo.pApplicationInfo = applicationInfo;

    // extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensionNames.size());
    createInfo.ppEnabledExtensionNames = (enabledExtensionNames.size() != 0) ? enabledExtensionNames.data() : VK_NULL_HANDLE;
    
    // layers
    createInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayerNames.size());
    createInfo.ppEnabledLayerNames = (enabledLayerNames.size() != 0) ? enabledLayerNames.data() : VK_NULL_HANDLE;
    
    // extend instance
    createInfo.pNext = next;

    return createInfo;
}
