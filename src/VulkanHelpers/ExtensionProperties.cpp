#include "VulkanHelpers/ExtensionProperties.hpp"

std::vector<ExtensionProperties> ExtensionProperties::Enumerate() {
    uint32_t count = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance extensions properties (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Instance extensions properties enumerated successfully (1st call, count: " << count << ")" << std::endl;

    std::vector<ExtensionProperties> properties(count, ExtensionProperties {});
    result = vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &count, properties.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance extensions properties (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Instance extensions properties enumerated successfully (2nd call, retrieved in array)" << std::endl;

    return properties;
}

std::ostream& operator << (std::ostream& out, ExtensionProperties const& extensionProperties) {
    out << "\tInstance extension found:" << std::endl;
    out << "\t\t - Name:                      "
        << extensionProperties.extensionName << std::endl; 
    out << "\t\t - Specification version:     "
        << extensionProperties.specVersion << std::endl;
        
    return out;
}

std::ostream& operator << (std::ostream& out, std::span<ExtensionProperties> extensionsProperties) {
    for (ExtensionProperties const& instanceExtensionProperties : extensionsProperties) {
        out << instanceExtensionProperties;
    }

    return out;
}