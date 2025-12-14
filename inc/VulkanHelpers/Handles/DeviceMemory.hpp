#ifndef VK_WRAPPER_DEVICE_MEMORY_HPP
#define VK_WRAPPER_DEVICE_MEMORY_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class DeviceMemory {
    public:
        DeviceMemory(VkMemoryAllocateInfo const& allocateInfo, Device* device);
        DeviceMemory(DeviceMemory const& other) = delete;
        DeviceMemory(DeviceMemory&& other);
        ~DeviceMemory();

        DeviceMemory& operator = (DeviceMemory&& other);

        VkDeviceMemory Handle() { return _handle; }
        VkDeviceMemory Handle() const { return _handle; }

        void Map(VkMemoryMapInfo const& memoryMapInfo, void** data);
        void Unmap(VkMemoryUnmapInfo const& memoryUnmapInfo);

    private:
        VkDeviceMemory _handle = VK_NULL_HANDLE;
        Device* _device;
};

#endif // VK_WRAPPER_DEVICE_MEMORY_HPP