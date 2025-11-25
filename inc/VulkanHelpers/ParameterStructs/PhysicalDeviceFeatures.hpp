#ifndef VK_WRAPPER_PHYSICAL_DEVICE_FEATURES_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_FEATURES_HPP

#include <iostream>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"

VkPhysicalDeviceFeatures2 GeneratePhysicalDeviceFeatures(PhysicalDevice const& physicalDevice);
std::ostream& operator << (std::ostream& out, VkPhysicalDeviceFeatures2 const& physicalDeviceFeatures);

#endif // VK_WRAPPER_PHYSICAL_DEVICE_FEATURES_HPP
