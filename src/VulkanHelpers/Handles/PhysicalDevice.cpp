#include "VulkanHelpers/Handles/PhysicalDevice.hpp"

#include "VulkanHelpers/Handles/Surface.hpp"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice) : _handle(physicalDevice) {}

VkSurfaceCapabilities2KHR PhysicalDevice::SurfaceCapabilities(VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) const {
    VkSurfaceCapabilities2KHR capabilities {};

    capabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
    capabilities.surfaceCapabilities = {};
    capabilities.pNext = VK_NULL_HANDLE;

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilities2KHR(_handle, &surfaceInfo, &capabilities);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get physical device surface capabilities (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical device surface capabilities retrieved successfully" << std::endl;

    return capabilities;
}

std::vector<VkSurfaceFormat2KHR> PhysicalDevice::SurfaceFormats(VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) const {
    uint32_t count = 0;
    VkResult result = vkGetPhysicalDeviceSurfaceFormats2KHR(_handle, &surfaceInfo, &count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get physical device surface formats (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical device surface formats retrieved successfully (1st call, count: " << count << ")" << std::endl;

    if (count == 0) {
        std::string error = "No surface formats found";
        throw std::runtime_error(error);
    }

    std::vector<VkSurfaceFormat2KHR> formats(count, GenerateSurfaceFormat());

    result = vkGetPhysicalDeviceSurfaceFormats2KHR(_handle, &surfaceInfo, &count, formats.data());
    if (result != VK_SUCCESS) {
        std::string error = "Could not get physical device surface formats (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical device surface formats retrieved successfully (2nd call, retrieved in array)" << std::endl;

    return formats;
} 

std::vector<VkPresentModeKHR> PhysicalDevice::PresentModes(Surface const& surface) const {
    uint32_t count = 0;
	VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(_handle, surface.Handle(), &count, VK_NULL_HANDLE);
	if (result != VK_SUCCESS) {
		std::string error = "Could not get physical device surface present modes (1st call, status: " + std::to_string(result) + ")";
		throw std::runtime_error(error);
	}
	//std::clog << "Physical device surface present modes retrieved successfully (1st call, count: " << count << ")" << std::endl;

	if (count == 0) {
		std::string error = "No present modes found";
		throw std::runtime_error(error);
	}

	std::vector<VkPresentModeKHR> presentModes(count, VkPresentModeKHR {});
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(_handle, surface.Handle(), &count, presentModes.data());
	if (result != VK_SUCCESS) {
		std::string error = "Could not get physical device surface present modes (2nd call, status: " + std::to_string(result) + ")";
		throw std::runtime_error(error);
	}
	//std::clog << "Physical device surface present modes retrieved successfully (2nd call, retrieved in array)" << std::endl;

	return presentModes;
}

std::vector<PhysicalDevice> PhysicalDevice::Enumerate(Instance const& instance) {
    // get physical devices count
    uint32_t count = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance.Handle(), &count, VK_NULL_HANDLE);
    if (result != VkResult::VK_SUCCESS) {
        std::string error = "Unable to enumerate physical devices (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical devices enumerated successfully (1st call, count: " << count << ")" << std::endl;

    // should not happen on a modern PC because the CPU can be used
    if (count == 0) {
        std::string error = "No physical devices found";
        throw std::runtime_error(error);
    }

    // enumerate physical devices
    std::vector<VkPhysicalDevice> tmpPhysicalDevices(count, VK_NULL_HANDLE);
    result = vkEnumeratePhysicalDevices(instance.Handle(), &count, tmpPhysicalDevices.data());
    if (result != VkResult::VK_SUCCESS) {
        std::string error = "Unable to enumerate physical devices (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical devices enumerated successfully (2nd call, retrieved in array)" << std::endl;

    std::vector<PhysicalDevice> physicalDevices {};
    physicalDevices.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        physicalDevices.emplace_back(tmpPhysicalDevices[i]);
    }

    return physicalDevices;
}

bool PhysicalDevice::IsSurfaceSupportedByQueueFamily(Surface const& surface, uint32_t queueFamilyIndex) const {
    VkBool32 supportedSurface = VK_FALSE;
    VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(_handle, queueFamilyIndex, surface.Handle(), &supportedSurface);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get surface support (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }

    return supportedSurface == VK_TRUE;
}
