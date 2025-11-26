#include "VulkanHelpers/ParameterStructs/SurfaceCapabilities.hpp"

VkSurfaceCapabilities2KHR GenerateSurfaceCapabilities(PhysicalDevice const& physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) {
    VkSurfaceCapabilities2KHR capabilities {};

    capabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
    capabilities.surfaceCapabilities = {};
    capabilities.pNext = VK_NULL_HANDLE;

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice.Handle(), &surfaceInfo, &capabilities);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get physical device surface capabilities (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical device surface capabilities retrieved successfully" << std::endl;

    return capabilities;
}

std::ostream& operator << (std::ostream& out, VkSurfaceCapabilitiesKHR const& physicalDeviceSurfaceCapabilities) {
    out << " - Surface capabilities" << std::endl;
    out << " - Current extent height         " 
        << physicalDeviceSurfaceCapabilities.currentExtent.height << std::endl;
    out << " - Current extent width          "
        << physicalDeviceSurfaceCapabilities.currentExtent.width << std::endl;
    out << " - Current transform             " << std::endl;
    DisplaySurfaceTransformFlags(physicalDeviceSurfaceCapabilities.currentTransform);
    out << " - Max image array layers        "
        << physicalDeviceSurfaceCapabilities.maxImageArrayLayers << std::endl;
    out << " - Max image count               "
        << physicalDeviceSurfaceCapabilities.maxImageCount << std::endl;
    out << " - Max image extent height       "
        << physicalDeviceSurfaceCapabilities.maxImageExtent.height << std::endl;
    out << " - Max image extent width        "
        << physicalDeviceSurfaceCapabilities.maxImageExtent.width << std::endl;
    out << " - Min image count               "
        << physicalDeviceSurfaceCapabilities.minImageCount << std::endl;
    out << " - Supported composite alpha     "
        << physicalDeviceSurfaceCapabilities.supportedCompositeAlpha << std::endl;
    out << " - Supported transforms:" << std::endl;
    DisplaySurfaceTransformFlags(physicalDeviceSurfaceCapabilities.supportedTransforms);
    out << " - Supported usage flags:" << std::endl;
    DisplayImageUsageFlags(physicalDeviceSurfaceCapabilities.supportedUsageFlags);

    return out;
}
