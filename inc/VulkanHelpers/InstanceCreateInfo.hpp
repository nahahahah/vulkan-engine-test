#ifndef VK_WRAPPER_INSTANCE_CREATE_INFO_HP
#define VK_WRAPPER_INSTANCE_CREATE_INFO_HP

#include <memory>
#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/ApplicationInfo.hpp"

struct InstanceCreateInfo : public VkInstanceCreateInfo {
    public:
        InstanceCreateInfo(
            std::unique_ptr<ApplicationInfo> const& applicationInfo,
            std::span<char const*>                  enabledExtensionNames,
            std::span<char const*>                  enabledLayerNames,
            uint32_t                                flags = 0,
            void*                                   next = VK_NULL_HANDLE
        );
};

#endif // VK_WRAPPER_INSTANCE_CREATE_INFO_HP