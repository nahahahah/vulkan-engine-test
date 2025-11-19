#ifndef VK_WRAPPER_SURFACE_FORMAT_HPP
#define VK_WRAPPER_SURFACE_FORMAT_HPP

#include <iostream>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/ColorSpace.hpp"

void DisplaySurfaceFormat(VkSurfaceFormatKHR const& surfaceFormat);

#endif // VK_WRAPPER_SURFACE_FORMAT_HPP
