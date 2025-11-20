#include "VulkanHelpers/SurfaceFormat.hpp"

VkSurfaceFormat2KHR GenerateSurfaceFormat() {
    VkSurfaceFormat2KHR format {};
    
    format.sType = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR;
    format.surfaceFormat = {};
    format.pNext = VK_NULL_HANDLE;

    return format;
}

std::vector<VkSurfaceFormat2KHR> EnumerateSurfaceFormats(PhysicalDevice const& physicalDevice, VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) {
    uint32_t count = 0;
    VkResult result = vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice.Handle(), &surfaceInfo, &count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get physical device surface formats (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Physical device surface formats retrieved successfully (1st call, count: " << count << ")" << std::endl;

    if (count == 0) {
        std::string error = "No surface formats found";
        throw std::runtime_error(error);
    }

    std::vector<VkSurfaceFormat2KHR> formats(count, GenerateSurfaceFormat());

    result = vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice.Handle(), &surfaceInfo, &count, formats.data());
    if (result != VK_SUCCESS) {
        std::string error = "Could not get physical device surface formats (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Physical device surface formats retrieved successfully (2nd call, retrieved in array)" << std::endl;

    return formats;
}

std::ostream& operator << (std::ostream& out, VkSurfaceFormatKHR const& surfaceFormat) {
    out << surfaceFormat.colorSpace << std::endl;
    out << " - Format          "
        << surfaceFormat.format << std::endl;

    return out;
}
