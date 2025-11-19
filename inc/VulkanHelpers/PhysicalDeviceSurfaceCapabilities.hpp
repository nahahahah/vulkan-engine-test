#ifndef VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_CAPABILITIES_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_CAPABILITIES_HPP

#include <iostream>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/SurfaceTransformFlags.hpp"
#include "VulkanHelpers/ImageUsageFlags.hpp"

void DisplayPhysicalDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR const& physicalDeviceSurfaceCapabilities);

#endif // VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_CAPABILITIES_HPP
