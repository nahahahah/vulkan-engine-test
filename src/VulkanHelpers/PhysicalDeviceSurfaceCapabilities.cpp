#include "VulkanHelpers/PhysicalDeviceSurfaceCapabilities.hpp"

void DisplayPhysicalDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR const& physicalDeviceSurfaceCapabilities) {
    std::clog << "\t - Surface capabilities" << std::endl;
    std::clog << "\t\t - Current extent height         " 
              << physicalDeviceSurfaceCapabilities.currentExtent.height << std::endl;
    std::clog << "\t\t - Current extent width          "
              << physicalDeviceSurfaceCapabilities.currentExtent.width << std::endl;
    std::clog << "\t\t - Current transforms:" << std::endl;
    DisplaySurfaceTransformFlagsKHR(physicalDeviceSurfaceCapabilities.currentTransform);
    std::clog << "\t\t - Max image array layers        "
              << physicalDeviceSurfaceCapabilities.maxImageArrayLayers << std::endl;
    std::clog << "\t\t - Max image count               "
              << physicalDeviceSurfaceCapabilities.maxImageCount << std::endl;
    std::clog << "\t\t - Max image extent height       "
              << physicalDeviceSurfaceCapabilities.maxImageExtent.height << std::endl;
    std::clog << "\t\t - Max image extent width        "
              << physicalDeviceSurfaceCapabilities.maxImageExtent.width << std::endl;
    std::clog << "\t\t - Min image count               "
              << physicalDeviceSurfaceCapabilities.minImageCount << std::endl;
    std::clog << "\t\t - Supported composite alpha     "
              << physicalDeviceSurfaceCapabilities.supportedCompositeAlpha << std::endl;
    std::clog << "\t\t - Supported transforms:" << std::endl;
    DisplaySurfaceTransformFlagsKHR(physicalDeviceSurfaceCapabilities.supportedTransforms);
    std::clog << "\t\t - Supported usage flags:" << std::endl;
    DisplayImageUsageFlags(physicalDeviceSurfaceCapabilities.supportedUsageFlags);
}
