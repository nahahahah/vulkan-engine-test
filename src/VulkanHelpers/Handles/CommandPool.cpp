#include "VulkanHelpers/Handles/CommandPool.hpp"

CommandPool::CommandPool(VkCommandPoolCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkCreateCommandPool(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create \"" + _label + "\" command pool (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" command pool created successfully: <VkCommandPool " << _handle << ">" << std::endl;
}

CommandPool::CommandPool(CommandPool&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

CommandPool::~CommandPool() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" command pool destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

CommandPool& CommandPool::operator = (CommandPool&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
