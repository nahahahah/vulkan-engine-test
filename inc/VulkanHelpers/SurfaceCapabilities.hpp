#ifndef VK_WRAPPER_SURFACE_CAPABILITIES_HPP
#define VK_WRAPPER_SURFACE_CAPABILITIES_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/PhysicalDevice.hpp"
#include "VulkanHelpers/CompositeAlphaFlags.hpp"
#include "VulkanHelpers/SurfaceTransformFlags.hpp"
#include "VulkanHelpers/ImageUsageFlags.hpp"

VkSurfaceCapabilities2KHR GenerateSurfaceCapabilities(PhysicalDevice const& physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo);
std::ostream& operator << (std::ostream& out, VkSurfaceCapabilitiesKHR const& surfaceCapabilities);

#endif // VK_WRAPPER_SURFACE_CAPABILITIES_HPP