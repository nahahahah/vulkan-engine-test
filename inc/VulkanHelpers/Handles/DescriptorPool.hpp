#ifndef VK_WRAPPER_DESCRIPTOR_POOL_HPP
#define VK_WRAPPER_DESCRIPTOR_POOL_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class DescriptorPool {
    public:
        DescriptorPool() = delete;
        DescriptorPool(VkDescriptorPoolCreateInfo const& createInfo, Device const& device, std::string const& label);
        DescriptorPool(DescriptorPool const& other) = delete;
        DescriptorPool(DescriptorPool&& other); 
        ~DescriptorPool();

        DescriptorPool& operator = (DescriptorPool const& other) = delete;
        DescriptorPool& operator = (DescriptorPool&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkDescriptorPool Handle() { return _handle; }
        VkDescriptorPool Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkDescriptorPool _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_DESCRIPTOR_POOL_HPP
