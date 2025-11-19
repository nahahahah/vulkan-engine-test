#ifndef VK_WRAPPER_EXTENSION_SUPPORT_CHECK_HPP
#define VK_WRAPPER_EXTENSION_SUPPORT_CHECK_HPP

#include <iostream>
#include <vector>
#include <cstring>

#include <vulkan/vulkan.h>

void DisplaySupportedExtensionsFromSDLVulkan(std::vector<const char*> const& enabledExtensions, std::vector<VkExtensionProperties> const& supportedExtensions);

#endif // VK_WRAPPER_EXTENSION_SUPPORT_CHECK_HPP
