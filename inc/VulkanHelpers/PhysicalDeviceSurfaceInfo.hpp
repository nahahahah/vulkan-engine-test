#ifndef VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_INFO_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Surface.hpp"

VkPhysicalDeviceSurfaceInfo2KHR GeneratePhysicalDeviceSurfaceInfo(Surface const& surface);

#endif // VK_WRAPPER_PHYSICAL_DEVICE_SURFACE_INFO_HPP