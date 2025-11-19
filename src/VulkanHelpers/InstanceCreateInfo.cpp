#include "VulkanHelpers/InstanceCreateInfo.hpp"

InstanceCreateInfo::InstanceCreateInfo(
    std::unique_ptr<ApplicationInfo> const& applicationInfo,
    std::span<char const*>                  enabledExtensionNames,
    std::span<char const*>                  enabledLayerNames,
    uint32_t                                flags,
    void*                                   next
) {
    // structure type and flags    
    this->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    this->flags = flags;

    // application info
    this->pApplicationInfo = applicationInfo.get();

    // extensions
    this->enabledExtensionCount = static_cast<uint32_t>(enabledExtensionNames.size());
    this->ppEnabledExtensionNames = (enabledExtensionNames.size() != 0) ? enabledExtensionNames.data() : VK_NULL_HANDLE;
    
    // layers
    this->enabledLayerCount = static_cast<uint32_t>(enabledLayerNames.size());
    this->ppEnabledLayerNames = (enabledLayerNames.size() != 0) ? enabledLayerNames.data() : VK_NULL_HANDLE;
    
    // extend instance
    this->pNext = next;
}