#ifndef VK_WRAPPER_QUEUE_FAMILY_PROPERTIES_HPP
#define VK_WRAPPER_QUEUE_FAMILY_PROPERTIES_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"
#include "VulkanHelpers/Flags/QueueFlags.hpp"

VkQueueFamilyProperties2 GenerateQueueFamilyProperties();
std::vector<VkQueueFamilyProperties2> EnumerateQueueFamilyProperties(PhysicalDevice const& physicalDevice);
std::ostream& operator << (std::ostream& out, VkQueueFamilyProperties2 const& queueFamilyProperties);

#endif // VK_WRAPPER_QUEUE_FAMILY_PROPERTIES_HPP
