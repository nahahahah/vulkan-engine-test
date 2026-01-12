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
        RenderPass(VkRenderPassCreateInfo2 const& createInfo, Device const& device, std::string const& label);
        RenderPass(RenderPass const& other) = delete;
        RenderPass(RenderPass&& other);
        ~RenderPass();

        RenderPass& operator = (RenderPass const& other) = delete;
        RenderPass& operator = (RenderPass&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }

        VkRenderPass Handle() { return _handle; }
        VkRenderPass Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkRenderPass _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_RENDER_PASS_HPP
