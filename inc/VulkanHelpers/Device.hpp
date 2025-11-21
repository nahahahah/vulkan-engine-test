#ifndef VK_WRAPPER_DEVICE_HPP
#define VK_WRAPPER_DEVICE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/PhysicalDevice.hpp"

class Device {
    public:
        Device(VkDeviceCreateInfo const& createInfo, PhysicalDevice const& physicalDevice);
        ~Device();

        VkDevice Handle() { return _handle; }
        VkDevice Handle() const { return _handle; }

    private:
        VkDevice _handle = VK_NULL_HANDLE;
};

#endif // VK_WRAPPER_DEVICE_HPP
