#ifndef VK_WRAPPER_SURFACE_CAPABILITIES_HPP
#define VK_WRAPPER_SURFACE_CAPABILITIES_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"
#include "VulkanHelpers/Flags/CompositeAlphaFlags.hpp"
#include "VulkanHelpers/Flags/SurfaceTransformFlags.hpp"
#include "VulkanHelpers/Flags/ImageUsageFlags.hpp"

VkSurfaceCapabilities2KHR GenerateSurfaceCapabilities(PhysicalDevice const& physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo);
std::ostream& operator << (std::ostream& out, VkSurfaceCapabilitiesKHR const& surfaceCapabilities);

#endif // VK_WRAPPER_SURFACE_CAPABILITIES_HPP
