#ifndef VK_WRAPPER_PIPELINE_HPP
#define VK_WRAPPER_PIPELINE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Device.hpp"

class Pipeline {
    public:
        Pipeline(VkGraphicsPipelineCreateInfo const& createInfo, Device const& device);
        ~Pipeline();

        VkPipeline Handle() { return _handle; }
        VkPipeline Handle() const { return _handle; }

    private:
        VkPipeline _handle = VK_NULL_HANDLE;
        Device const& _device;
};

#endif // VK_WRAPPER_PIPELINE_HPP
