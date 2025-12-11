#include "VulkanHelpers/Handles/DeviceMemory.hpp"

DeviceMemory::DeviceMemory(VkMemoryAllocateInfo const& allocateInfo, Device& device) : _device(device) {
    VkResult result = vkAllocateMemory(device.Handle(), &allocateInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not allocate memory (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Memory allocated successfully: <VkDeviceMemory " << _handle << ">" << std::endl;
}

DeviceMemory::~DeviceMemory() {
    std::clog << "Memory set for deallocation: <VkDeviceMemory " << _handle << ">" << std::endl;
    if (_handle != VK_NULL_HANDLE) {
        vkFreeMemory(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Memory freed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

void DeviceMemory::Map(VkMemoryMapInfo const& memoryMapInfo, void** data) {
    VkResult result = vkMapMemory2(_device.Handle(), &memoryMapInfo, data);
    if (result != VK_SUCCESS) {
        std::string error = "Could not map memory (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }

    std::clog << "Memory mapped successfully: <VkDeviceMemory " << _handle << ">" << std::endl;
}

void DeviceMemory::Unmap(VkMemoryUnmapInfo const& memoryUnmapInfo) {
    VkResult result = vkUnmapMemory2(_device.Handle(), &memoryUnmapInfo);
    if (result != VK_SUCCESS) {
        std::string error = "Could not unmap memory (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }

    std::clog << "Memory unmapped successfully: <VkDeviceMemory " << _handle << ">" << std::endl;
}
