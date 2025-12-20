#ifndef VK_WRAPPER_FRAMEBUFFER_HPP
#define VK_WRAPPER_FRAMEBUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Framebuffer {
    public:
        Framebuffer() = default;
        Framebuffer(VkFramebufferCreateInfo const& createInfo, Device& device);
        Framebuffer(Framebuffer const& other) = delete;
        Framebuffer(Framebuffer&& other);
        ~Framebuffer();

        Framebuffer& operator = (Framebuffer const& other) = delete;
        Framebuffer& operator = (Framebuffer&& other);

        VkFramebuffer Handle() { return _handle; }
        VkFramebuffer Handle() const { return _handle; }

        void CreateHandle(VkFramebufferCreateInfo const& createInfo);
        void DestroyHandle();

    private:
        VkFramebuffer _handle = VK_NULL_HANDLE;
        Device* _device = nullptr;
};

#endif // VK_WRAPPER_FRAMEBUFFER_HPP