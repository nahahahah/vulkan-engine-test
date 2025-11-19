#include "VulkanHelpers/SurfaceCapabilities.hpp"

VkSurfaceCapabilities2KHR GenerateSurfaceCapabilities() {
    VkSurfaceCapabilities2KHR capabilities {};

    capabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
    capabilities.surfaceCapabilities = {};
    capabilities.pNext = VK_NULL_HANDLE;

    return capabilities;
}
