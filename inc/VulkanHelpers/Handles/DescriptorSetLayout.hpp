#ifndef VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_HPP
#define VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class DescriptorSetLayout {
    public:
        DescriptorSetLayout() = default;
        DescriptorSetLayout(VkDescriptorSetLayoutCreateInfo const& createInfo, Device const& device, std::string const& label);
        DescriptorSetLayout(DescriptorSetLayout const& other) = delete;
        DescriptorSetLayout(DescriptorSetLayout&& other);
        ~DescriptorSetLayout();

        DescriptorSetLayout& operator = (DescriptorSetLayout const& other) = delete;
        DescriptorSetLayout& operator = (DescriptorSetLayout&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkDescriptorSetLayout Handle() { return _handle; }
        VkDescriptorSetLayout Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkDescriptorSetLayout _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_HPP
