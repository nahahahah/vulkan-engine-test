#ifndef VK_WRAPPER_APPLICATION_INFO_HPP
#define VK_WRAPPER_APPLICATION_INFO_HPP

#include <vulkan/vulkan.h>

struct ApplicationInfo : public VkApplicationInfo {
    public:
        ApplicationInfo();
};

#endif // VK_WRAPPER_APPLICATION_INFO_HPP
