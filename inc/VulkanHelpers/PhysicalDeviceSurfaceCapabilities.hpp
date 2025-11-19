#ifndef VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_CAPABILITIES_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_CAPABILITIES_HPP

#include <iostream>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/CompositeAlphaFlags.hpp"
#include "VulkanHelpers/SurfaceTransformFlags.hpp"
#include "VulkanHelpers/ImageUsageFlags.hpp"

std::ostream& operator << (std::ostream& out, VkSurfaceCapabilitiesKHR const& physicalDeviceSurfaceCapabilities);

#endif // VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_CAPABILITIES_HPP
