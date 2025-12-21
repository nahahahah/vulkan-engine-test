#ifndef VK_WRAPPER_PIPELINE_LAYOUT_HPP
#define VK_WRAPPER_PIPELINE_LAYOUT_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class PipelineLayout {
    public:
        PipelineLayout() = delete;
        PipelineLayout(VkPipelineLayoutCreateInfo const& createInfo, Device const& device);
        PipelineLayout(PipelineLayout const& other) = delete;
        PipelineLayout(PipelineLayout&& other);
        ~PipelineLayout();

        PipelineLayout& operator = (PipelineLayout const& other) = delete;
        PipelineLayout& operator = (PipelineLayout&& other);
        
        VkPipelineLayout Handle() { return _handle; }
        VkPipelineLayout Handle() const { return _handle; }

    private:
        VkPipelineLayout _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_PIPELINE_LAYOUT_HPP
