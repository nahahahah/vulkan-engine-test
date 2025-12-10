#include "VulkanHelpers/Handles/Framebuffer.hpp"

Framebuffer::Framebuffer(VkFramebufferCreateInfo const& createInfo, Device const& device) : _device(device) {
    try {
        CreateHandle(createInfo);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

Framebuffer::Framebuffer(Framebuffer&& other) : _device(other._device) {
    if (other._handle != VK_NULL_HANDLE) {
        _handle = other._handle;
        other._handle = VK_NULL_HANDLE;
    }
}

Framebuffer::~Framebuffer() {
    DestroyHandle();
}

void Framebuffer::CreateHandle(VkFramebufferCreateInfo const& createInfo) {
    VkResult result = vkCreateFramebuffer(_device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create frame buffer (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Frame buffer created successfully: <VkFramebuffer " << _handle << ">" << std::endl;
}

void Framebuffer::DestroyHandle() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Framebuffer destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}
