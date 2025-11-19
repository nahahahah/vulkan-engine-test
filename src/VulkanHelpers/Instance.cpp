#include "VulkanHelpers/Instance.hpp"

Instance::Instance(VkInstanceCreateInfo const& createInfo) {
    VkResult result = vkCreateInstance(&createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VkResult::VK_SUCCESS) {
        std::string error =  "Unable to create a Vulkan instance (result: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Vulkan instance created successfully: <VkInstance " << _handle << ">" << std::endl;
}

Instance::~Instance() {
    if (_handle != VK_NULL_HANDLE) {
        // destroy instance and enumerated physical devices
        vkDestroyInstance(_handle, VK_NULL_HANDLE);
        std::clog << "Vulkan instance destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}