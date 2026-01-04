#include "VulkanHelpers/Handles/Semaphore.hpp"

Semaphore::Semaphore(VkSemaphoreCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkCreateSemaphore(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create \"" + _label + "\" semaphore (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" semaphore created successfully: <VkSemaphore " << _handle << ">" << std::endl;
}

Semaphore::Semaphore(Semaphore&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

Semaphore::~Semaphore() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroySemaphore(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" semaphore destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

Semaphore& Semaphore::operator = (Semaphore&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
