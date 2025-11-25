#ifndef VK_WRAPPER_PRESENT_MODE_HPP
#define VK_WRAPPER_PRESENT_MODE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"
#include "VulkanHelpers/Handles/Surface.hpp"

std::vector<VkPresentModeKHR> EnumeratePresentModes(PhysicalDevice const& physicalDevice, Surface const& surface);
void DisplayPresentMode(VkPresentModeKHR presentMode);

#endif // VK_WRAPPER_PRESENT_MODE_HPP
