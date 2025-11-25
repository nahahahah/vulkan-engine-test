#include "VulkanHelpers/Handles/Framebuffer.hpp"

Framebuffer::Framebuffer(VkFramebufferCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateFramebuffer(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create frame buffer (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Frame buffer created successfully: <VkFramebuffer " << _handle << ">" << std::endl;
}

Framebuffer::Framebuffer(Framebuffer&& other) : _device(other._device) {
    if (other._handle != VK_NULL_HANDLE) {
        _handle = other._handle;
        other._handle = VK_NULL_HANDLE;
    }
}

Framebuffer::~Framebuffer() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Framebuffer destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}
