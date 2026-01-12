#ifndef VK_WRAPPER_DEVICE_MEMORY_HPP
#define VK_WRAPPER_DEVICE_MEMORY_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class DeviceMemory {
    public:
        DeviceMemory() = delete;
        DeviceMemory(VkMemoryAllocateInfo const& allocateInfo, Device const& device, std::string const& label);
        DeviceMemory(DeviceMemory const& other) = delete;
        DeviceMemory(DeviceMemory&& other);
        ~DeviceMemory();

        DeviceMemory& operator = (DeviceMemory const& other) = delete;
        DeviceMemory& operator = (DeviceMemory&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }

        VkDeviceMemory Handle() { return _handle; }
        VkDeviceMemory Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkDeviceMemory _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_DEVICE_MEMORY_HPP
