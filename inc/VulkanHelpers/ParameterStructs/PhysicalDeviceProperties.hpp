#ifndef VK_WRAPPER_PHYSICAL_DEVICE_PROPERTIES_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_PROPERTIES_HPP

#include <iostream>
#include <iomanip>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/ParameterStructs/Version.hpp"
#include "VulkanHelpers/Handles/PhysicalDevice.hpp"
#include "VulkanHelpers/ParameterStructs/PhysicalDeviceLimits.hpp"
#include "VulkanHelpers/ParameterStructs/PhysicalDeviceSparseProperties.hpp"

VkPhysicalDeviceProperties2 GeneratePhysicalDeviceProperties(PhysicalDevice const& physicalDevice);
std::ostream& operator << (std::ostream& out, VkPhysicalDeviceProperties2 const& physicalDeviceProperties);

#endif // VK_WRAPPER_PHYSICAL_DEVICE_PROPERTIES_HPP
