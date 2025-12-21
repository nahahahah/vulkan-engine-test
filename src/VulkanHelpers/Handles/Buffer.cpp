#include "VulkanHelpers/Handles/Buffer.hpp"

#include "VulkanHelpers/Handles/Device.hpp"

Buffer::Buffer(VkBufferCreateInfo const& createInfo, Device const& device) : _device(&device) {
    VkResult result = vkCreateBuffer(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create a buffer (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Buffer created successfully: <VkBuffer " << _handle << ">" << std::endl;
}

Buffer::Buffer(Buffer&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
} 

Buffer::~Buffer() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyBuffer(_device->Handle(), _handle, VK_NULL_HANDLE);
        _handle = VK_NULL_HANDLE;
    }
}

Buffer& Buffer::operator = (Buffer&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
