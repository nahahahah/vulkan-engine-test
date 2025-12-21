#include "VulkanHelpers/Handles/Fence.hpp"

Fence::Fence(VkFenceCreateInfo const& createInfo, Device const& device) : _device(&device) {
    VkResult result = vkCreateFence(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create fence (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Fence created successfully: <VkFence " << _handle << ">" << std::endl;
}

Fence::Fence(Fence&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

Fence::~Fence() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFence(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Fence destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    } 
}

Fence& Fence::operator = (Fence&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
