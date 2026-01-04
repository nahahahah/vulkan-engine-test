#include "VulkanHelpers/Handles/Fence.hpp"

Fence::Fence(VkFenceCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkCreateFence(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create \"" + _label + "\" fence (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" fence created successfully: <VkFence " << _handle << ">" << std::endl;
}

Fence::Fence(Fence&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

Fence::~Fence() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFence(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" fence destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    } 
}

Fence& Fence::operator = (Fence&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
