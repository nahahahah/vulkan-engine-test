#include "VulkanHelpers/LayerProperties.hpp"

std::vector<LayerProperties> LayerProperties::Enumerate() {
    uint32_t count = 0;
    VkResult result = vkEnumerateInstanceLayerProperties(&count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance layer properties (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Instance layer properties enumerated successfully (1st call, count: " << count << ")" << std::endl;
    
    std::vector<LayerProperties> properties(count, LayerProperties {});
    result = vkEnumerateInstanceLayerProperties(&count, properties.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to enumerate instance layer properties (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Instance layer properties enumerated successfully (2nd call, retrieved in array)" << std::endl;

    return properties;
}

bool LayerProperties::ValidationLayerSupported(std::span<char const*> validationLayers, std::span<LayerProperties> layersProperties) {
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

void LayerProperties::DisplayLayerProperties(LayerProperties const& layerProperties) {
    std::cout << "\tInstance layer properties found:" << std::endl;
    std::cout << "\t\t - Description                "
              << layerProperties.description << std::endl;
    std::cout << "\t\t - Implementation version     " 
              << layerProperties.implementationVersion << std::endl;
    std::cout << "\t\t - Name                       "
              << layerProperties.layerName << std::endl;
    Version specVersion(layerProperties.specVersion);
    std::cout << "\t\t - Specification version      "
              << specVersion << std::endl;
}

void LayerProperties::DisplayLayersProperties(std::span<LayerProperties> layersProperties) {
    for (LayerProperties const& instanceLayerProperties : layersProperties) {
        DisplayLayerProperties(instanceLayerProperties);
    }
}