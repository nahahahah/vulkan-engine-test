#include "VulkanHelpers/ExtensionProperties.hpp"

std::vector<VkExtensionProperties> EnumerateInstanceExtensionProperties() {
    uint32_t count = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance extensions properties (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Instance extensions properties enumerated successfully (1st call, count: " << count << ")" << std::endl;

    std::vector<VkExtensionProperties> properties(count, VkExtensionProperties {});
    result = vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &count, properties.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance extensions properties (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Instance extensions properties enumerated successfully (2nd call, retrieved in array)" << std::endl;

    return properties;
}

std::vector<VkExtensionProperties> EnumerateDeviceExtensionProperties(PhysicalDevice const& physicalDevice) {
    uint32_t count = 0;
    VkResult result = vkEnumerateDeviceExtensionProperties(physicalDevice.Handle(), VK_NULL_HANDLE, &count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get device extensions properties (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Device extensions properties enumerated successfully (1st call, count: " << count << ")" << std::endl;

    std::vector<VkExtensionProperties> properties(count, VkExtensionProperties {});
    result = vkEnumerateDeviceExtensionProperties(physicalDevice.Handle(), VK_NULL_HANDLE, &count, properties.data());
    if (result != VK_SUCCESS) {
        std::string error = "Could not get device extensions properties (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Device extensions properties enumerated successfully (2nd call, retrieved in array)" << std::endl;

    return properties;
}

bool AreDeviceExtensionsSupported(std::span<char const*> deviceExtensions, std::span<VkExtensionProperties> deviceExtensionsProperties) {
    std::set<std::string> requiredDeviceExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (VkExtensionProperties const& deviceExtensionProperties : deviceExtensionsProperties) {
        requiredDeviceExtensions.erase(deviceExtensionProperties.extensionName);
    }

    return requiredDeviceExtensions.empty();
}

std::ostream& operator << (std::ostream& out, VkExtensionProperties const& extensionProperties) {
    out << "\tInstance extension found:" << std::endl;
    out << "\t\t - Name:                      "
        << extensionProperties.extensionName << std::endl; 
    out << "\t\t - Specification version:     "
        << extensionProperties.specVersion << std::endl;
        
    return out;
}

std::ostream& operator << (std::ostream& out, std::span<VkExtensionProperties> extensionsProperties) {
    for (VkExtensionProperties const& instanceExtensionProperties : extensionsProperties) {
        out << instanceExtensionProperties;
    }

    return out;
}
