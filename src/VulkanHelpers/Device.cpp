#include "VulkanHelpers/Device.hpp"

Device::Device(VkDeviceCreateInfo const& createInfo, PhysicalDevice const& physicalDevice) {
    VkResult result = vkCreateDevice(physicalDevice.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a device (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Device created successully: <VkDevice " << _handle << ">" << std::endl;
}

Device::~Device() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyDevice(_handle, VK_NULL_HANDLE);
        std::clog << "Device destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}
