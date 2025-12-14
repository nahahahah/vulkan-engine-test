#include "VulkanHelpers/Handles/Buffer.hpp"

Buffer::Buffer(VkBufferCreateInfo const& createInfo, Device* device) : _device(device) {
    VkResult result = vkCreateBuffer(device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
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
    _device = nullptr;

    return *this;
}

VkMemoryRequirements2 Buffer::MemoryRequirements(VkBufferMemoryRequirementsInfo2 info) {
    VkMemoryRequirements2 requirements {};

    // structure type
    requirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2; 

    vkGetBufferMemoryRequirements2(_device->Handle(), &info, &requirements);

    // extend requirements
    requirements.pNext = VK_NULL_HANDLE;

    return requirements;
}
