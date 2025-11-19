#ifndef VK_WRAPPER_PHYSICAL_DEVICE_PROPERTIES_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_PROPERTIES_HPP

#include <iostream>
#include <iomanip>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Version.hpp"
#include "VulkanHelpers/PhysicalDevice.hpp"
#include "VulkanHelpers/PhysicalDeviceLimits.hpp"
#include "VulkanHelpers/PhysicalDeviceSparseProperties.hpp"

struct PhysicalDeviceProperties : public VkPhysicalDeviceProperties2 {
    PhysicalDeviceProperties(PhysicalDevice const& physicalDevice);

    friend std::ostream& operator << (std::ostream& out, VkPhysicalDeviceProperties2 const& physicalDeviceProperties);
};

#endif // VK_WRAPPER_PHYSICAL_DEVICE_PROPERTIES_HPP
