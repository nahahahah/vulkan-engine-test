#ifndef VK_WRAPPER_RENDER_PASS_HPP
#define VK_WRAPPER_RENDER_PASS_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class RenderPass {
    public:
        RenderPass(VkRenderPassCreateInfo2 const& createInfo, Device const& device);
        ~RenderPass();

        VkRenderPass Handle() { return _handle; }
        VkRenderPass Handle() const { return _handle; }

    private:
        VkRenderPass _handle = VK_NULL_HANDLE;
        Device const& _device;
};

#endif // VK_WRAPPER_RENDER_PASS_HPP
