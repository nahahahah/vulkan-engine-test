#ifndef VK_WRAPPER_RENDER_PASS_HPP
#define VK_WRAPPER_RENDER_PASS_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class RenderPass {
    public:
        RenderPass() = delete;
        RenderPass(VkRenderPassCreateInfo2 const& createInfo, Device const& device);
        RenderPass(RenderPass const& other) = delete;
        RenderPass(RenderPass&& other);
        ~RenderPass();

        RenderPass& operator = (RenderPass const& other) = delete;
        RenderPass& operator = (RenderPass&& other);

        VkRenderPass Handle() { return _handle; }
        VkRenderPass Handle() const { return _handle; }

    private:
        VkRenderPass _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_RENDER_PASS_HPP
