#ifndef VK_WRAPPER_INSTANCE_HPP
#define VK_WRAPPER_INSTANCE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/InstanceCreateInfo.hpp"

class Instance {
    public:
        Instance(InstanceCreateInfo const& createInfo);
        ~Instance();

        VkInstance Handle() { return _handle; }
        VkInstance Handle() const { return _handle; }

    private:
        VkInstance _handle = VK_NULL_HANDLE;
};

#endif // VK_WRAPPER_INSTANCE_HPP