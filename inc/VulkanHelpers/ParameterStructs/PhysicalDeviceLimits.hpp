#ifndef VK_WRAPPER_PHYSICAL_DEVICE_LIMITS_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_LIMITS_HPP

#include <iostream>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Flags/SampleCountFlags.hpp"

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceLimits const& limits);

struct PhysicalDeviceLimits : public VkPhysicalDeviceLimits {
    PhysicalDeviceLimits() = default;

    friend std::ostream& operator << (std::ostream& out, VkPhysicalDeviceLimits const& limits);
};

#endif // VK_WRAPPER_PHYSICAL_DEVICE_LIMITS_HPP
