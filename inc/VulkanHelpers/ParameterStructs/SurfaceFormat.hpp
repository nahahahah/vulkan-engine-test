#ifndef VK_WRAPPER_SURFACE_FORMAT_HPP
#define VK_WRAPPER_SURFACE_FORMAT_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"
#include "VulkanHelpers/ParameterEnums/ColorSpace.hpp"

VkSurfaceFormat2KHR GenerateSurfaceFormat();
std::vector<VkSurfaceFormat2KHR> EnumerateSurfaceFormats(PhysicalDevice const& physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo);
std::ostream& operator << (std::ostream& out, VkSurfaceFormatKHR const& surfaceFormat);

#endif // VK_WRAPPER_SURFACE_FORMAT_HPP
