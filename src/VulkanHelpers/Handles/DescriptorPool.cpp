#include "VulkanHelpers/Handles/DescriptorPool.hpp"

DescriptorPool::DescriptorPool(VkDescriptorPoolCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkCreateDescriptorPool(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create \"" + _label + "\" descriptor pool (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Descriptor pool created successfully: <VkDescriptorPool " << _handle << ">" << std::endl;
}

DescriptorPool::DescriptorPool(DescriptorPool&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

DescriptorPool::~DescriptorPool() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(_device->Handle(), _handle, VK_NULL_HANDLE);
        _handle = VK_NULL_HANDLE;
        std::clog << "Descriptor pool destroyed successfully" << std::endl;
    }
}

DescriptorPool& DescriptorPool::operator = (DescriptorPool&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
