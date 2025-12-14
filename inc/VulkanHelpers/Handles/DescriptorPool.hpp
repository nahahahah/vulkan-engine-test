#ifndef VK_WRAPPER_DESCRIPTOR_POOL_HPP
#define VK_WRAPPER_DESCRIPTOR_POOL_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class DescriptorPool {
    public:
        DescriptorPool(VkDescriptorPoolCreateInfo const& createInfo, Device& device);
        ~DescriptorPool();

        VkDescriptorPool Handle() { return _handle; }
        VkDescriptorPool Handle() const { return _handle; }

    private:
        VkDescriptorPool _handle = VK_NULL_HANDLE;
        Device* _device = nullptr;
};

#endif // VK_WRAPPER_DESCRIPTOR_POOL_HPP
