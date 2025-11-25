#ifndef VK_WRAPPER_EXTENSION_PROPERTIES_HPP
#define VK_WRAPPER_EXTENSION_PROPERTIES_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <span>
#include <string>
#include <set>
#include <cstring>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"

std::vector<VkExtensionProperties> EnumerateInstanceExtensionProperties();
std::vector<VkExtensionProperties> EnumerateDeviceExtensionProperties(PhysicalDevice const& physicalDevice);
bool AreDeviceExtensionsSupported(std::span<char const*> requiredDeviceExtensions, std::span<VkExtensionProperties> deviceExtensionsProperties);
std::ostream& operator << (std::ostream& out, VkExtensionProperties const& extensionProperties);
std::ostream& operator << (std::ostream& out, std::span<VkExtensionProperties> extensionsProperties);
void DisplaySupportedExtensionsFromSDLVulkan(std::vector<const char*> const& enabledExtensions, std::vector<VkExtensionProperties> const& supportedExtensions);

#endif // VK_WRAPPER_EXTENSION_PROPERTIES_HPP
