#ifndef VK_WRAPPER_FRAMEBUFFER_HPP
#define VK_WRAPPER_FRAMEBUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Framebuffer {
    public:
        Framebuffer() = delete;
        Framebuffer(VkFramebufferCreateInfo const& createInfo, Device const& device, std::string const& label);
        Framebuffer(Framebuffer const& other) = delete;
        Framebuffer(Framebuffer&& other);
        ~Framebuffer();

        Framebuffer& operator = (Framebuffer const& other) = delete;
        Framebuffer& operator = (Framebuffer&& other);
        
        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }

        VkFramebuffer Handle() { return _handle; }
        VkFramebuffer Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkFramebuffer _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_FRAMEBUFFER_HPP
