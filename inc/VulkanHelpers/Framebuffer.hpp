#ifndef VK_WRAPPER_FRAMEBUFFER_HPP
#define VK_WRAPPER_FRAMEBUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Device.hpp"

class Framebuffer {
    public:
        Framebuffer(VkFramebufferCreateInfo const& createInfo, Device const& device);
        ~Framebuffer();

        VkFramebuffer Handle() { return _handle; }
        VkFramebuffer Handle() const { return _handle; }

    private:
        VkFramebuffer _handle = VK_NULL_HANDLE;
        Device const& _device;
};

#endif // VK_WRAPPER_FRAMEBUFFER_HPP