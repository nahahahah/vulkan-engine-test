#include "VulkanHelpers/Handles/Buffer.hpp"

#include "VulkanHelpers/Handles/Device.hpp"

Buffer::Buffer(VkBufferCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _device(&device), _label(label) {
    VkResult result = vkCreateBuffer(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create the \"" + _label + "\" buffer (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" buffer created successfully: <VkBuffer " << _handle << ">" << std::endl;
}

Buffer::Buffer(Buffer&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
} 

Buffer::~Buffer() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyBuffer(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" buffer destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

Buffer& Buffer::operator = (Buffer&& other) {
    _label = other._label;
    other._label = "";
    
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
