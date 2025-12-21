#ifndef VK_WRAPPER_PIPELINE_HPP
#define VK_WRAPPER_PIPELINE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Pipeline {
    public:
        Pipeline() = delete;
        Pipeline(VkGraphicsPipelineCreateInfo const& createInfo, Device const& device);
        Pipeline(Pipeline const& other) = delete;
        Pipeline(Pipeline&& other);
        ~Pipeline();

        Pipeline& operator = (Pipeline const& other) = delete;
        Pipeline& operator = (Pipeline&& other);

        VkPipeline Handle() { return _handle; }
        VkPipeline Handle() const { return _handle; }

    private:
        VkPipeline _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_PIPELINE_HPP
