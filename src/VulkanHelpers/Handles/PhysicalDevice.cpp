#include "VulkanHelpers/Handles/PhysicalDevice.hpp"

#include "VulkanHelpers/Handles/Surface.hpp"
#include "VulkanHelpers/ParameterStructs/QueueFamilyProperties.hpp"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice) : _handle(physicalDevice) {}

PhysicalDevice::PhysicalDevice(PhysicalDevice&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;
}

PhysicalDevice& PhysicalDevice::operator = (PhysicalDevice&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    return *this;
}

VkPhysicalDeviceProperties2 PhysicalDevice::Properties() const {
    VkPhysicalDeviceProperties2 properties {};
    
    // structure type
    properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;

    // physical device properties
    properties.properties = {};

    vkGetPhysicalDeviceProperties2(_handle, &properties);

    // extend properties
    properties.pNext = VK_NULL_HANDLE;

    return properties;
}

VkPhysicalDeviceFeatures2 PhysicalDevice::Features() const {
    VkPhysicalDeviceFeatures2 features {};

    // structure type
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    
    // physical device features
    features.features = {};

    vkGetPhysicalDeviceFeatures2(_handle, &features);

    // extend features
    features.pNext = VK_NULL_HANDLE;

    return features;
}

std::vector<VkQueueFamilyProperties2> PhysicalDevice::QueueFamilyProperties() const {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties2(_handle, &count, VK_NULL_HANDLE);
    if (count == 0) {
        std::string error = "Could not find any queue family properties";
        throw std::runtime_error(error);
    }

    std::vector<VkQueueFamilyProperties2> properties(count, GenerateQueueFamilyProperties());
    vkGetPhysicalDeviceQueueFamilyProperties2(_handle, &count, properties.data());

    return properties;
}

VkSurfaceCapabilities2KHR PhysicalDevice::SurfaceCapabilities(VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) const {
    VkSurfaceCapabilities2KHR capabilities {};

    // structure type
    capabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;

    capabilities.surfaceCapabilities = {};

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilities2KHR(_handle, &surfaceInfo, &capabilities);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get physical device surface capabilities (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical device surface capabilities retrieved successfully" << std::endl;

    // extend capabilities
    capabilities.pNext = VK_NULL_HANDLE;

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

VkFormatProperties2 PhysicalDevice::FormatProperties(VkFormat format) {
    auto formatProperties = GenerateFormatProperties();
    vkGetPhysicalDeviceFormatProperties2(_handle, format, &formatProperties);

    return formatProperties;
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

VkPhysicalDeviceMemoryProperties2 PhysicalDevice::MemoryProperties() const {
    VkPhysicalDeviceMemoryProperties2 properties {};

    // structure type
    properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;

    properties.memoryProperties = {};
    
    vkGetPhysicalDeviceMemoryProperties2(_handle, &properties);

    // extend properties
    properties.pNext = VK_NULL_HANDLE;

    return properties;
}

PhysicalDeviceCollection::PhysicalDeviceCollection(Instance const& instance) {
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
    _handles.resize(count);
    result = vkEnumeratePhysicalDevices(instance.Handle(), &count, _handles.data());
    if (result != VkResult::VK_SUCCESS) {
        std::string error = "Unable to enumerate physical devices (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Physical devices enumerated successfully (2nd call, retrieved in array)" << std::endl;

    _wrappers.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        _wrappers.emplace_back(_handles[i]);
    }
}

PhysicalDevice& PhysicalDeviceCollection::operator [] (size_t index) {
    assert("`index` must be within the bounds of the container" &&
           index < _wrappers.size());

    return _wrappers[index];
}

PhysicalDevice const& PhysicalDeviceCollection::operator [] (size_t index) const {
    assert("`index` must be within the bounds of the container" &&
           index < _wrappers.size());

    return _wrappers[index];
}
