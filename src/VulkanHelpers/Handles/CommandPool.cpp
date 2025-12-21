#include "VulkanHelpers/Handles/CommandPool.hpp"

CommandPool::CommandPool(VkCommandPoolCreateInfo const& createInfo, Device const& device) : _device(&device) {
    VkResult result = vkCreateCommandPool(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create command pool (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Command pool created successfully: <VkCommandPool " << _handle << ">" << std::endl;
}

CommandPool::CommandPool(CommandPool&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

CommandPool::~CommandPool() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Command pool destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

CommandPool& CommandPool::operator = (CommandPool&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
