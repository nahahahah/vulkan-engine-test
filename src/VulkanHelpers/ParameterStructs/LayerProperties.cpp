#include "VulkanHelpers/ParameterStructs/LayerProperties.hpp"

std::vector<VkLayerProperties> EnumerateLayerProperties() {
    uint32_t count = 0;
    VkResult result = vkEnumerateInstanceLayerProperties(&count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance layer properties (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Instance layer properties enumerated successfully (1st call, count: " << count << ")" << std::endl;
    
    std::vector<VkLayerProperties> properties(count, VkLayerProperties {});
    result = vkEnumerateInstanceLayerProperties(&count, properties.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance layer properties (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Instance layer properties enumerated successfully (2nd call, retrieved in array)" << std::endl;

    return properties;
}

bool AreValidationLayerSupported(std::span<char const*> validationLayers, std::span<VkLayerProperties> layersProperties) {
    bool supported = true;
    for (char const* layerName : validationLayers) {
        bool layerFound = false;

        for (VkLayerProperties const& instanceLayerProperties : layersProperties) {
            if (strcmp(layerName, instanceLayerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            supported = false;
            break;
        }
    }

    return supported;
}

std::ostream& operator << (std::ostream& out, VkLayerProperties const& layerProperties) {
    out << "Instance layer properties found:" << std::endl;
    out << " - Description                "
        << layerProperties.description << std::endl;
    out << " - Implementation version     " 
        << layerProperties.implementationVersion << std::endl;
    out << " - Name                       "
        << layerProperties.layerName << std::endl;
    Version specVersion(layerProperties.specVersion);
    out << " - Specification version      "
        << specVersion << std::endl;

    return out;
}

std::ostream& operator << (std::ostream& out, std::span<VkLayerProperties> layersProperties) {
    for (VkLayerProperties const& instanceLayerProperties : layersProperties) {
        out << instanceLayerProperties << std::endl;
    }

    return out;
}
