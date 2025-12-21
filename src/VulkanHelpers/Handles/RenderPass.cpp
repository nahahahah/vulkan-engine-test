#include "VulkanHelpers/Handles/RenderPass.hpp"

RenderPass::RenderPass(VkRenderPassCreateInfo2 const& createInfo, Device const& device) : _device(&device) {
    VkResult result = vkCreateRenderPass2(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create a render pass (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Render pass created successfully: <VkRenderPass " << _handle << ">" << std::endl;
}

RenderPass::RenderPass(RenderPass&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

RenderPass::~RenderPass() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyRenderPass(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Render pass destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

RenderPass& RenderPass::operator = (RenderPass&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
