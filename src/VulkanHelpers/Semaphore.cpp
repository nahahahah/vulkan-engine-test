#include "VulkanHelpers/Semaphore.hpp"

Semaphore::Semaphore(VkSemaphoreCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateSemaphore(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create semaphore (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Semaphore created successfully: <VkSemaphore " << _handle << ">" << std::endl;
}

Semaphore::~Semaphore() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroySemaphore(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Semaphore destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}
