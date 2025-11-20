#ifndef VK_WRAPPER_PHYSICAL_DEVICE_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Instance.hpp"

class PhysicalDevice {
    public:
        PhysicalDevice() = default;
        PhysicalDevice(VkPhysicalDevice const& physicalDevice);

        VkPhysicalDevice Handle() { return _handle; }
        VkPhysicalDevice Handle() const { return _handle; }

        static std::vector<PhysicalDevice> Enumerate(Instance const& instance);

    private:
        VkPhysicalDevice _handle = nullptr;
};

#endif // VK_WRAPPER_PHYSICAL_DEVICE_HPP