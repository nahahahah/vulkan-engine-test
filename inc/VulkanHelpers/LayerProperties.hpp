#ifndef VK_WRAPPER_LAYER_PROPERTIES_HPP
#define VK_WRAPPER_LAYER_PROPERTIES_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <span>
#include <string>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Version.hpp"

struct LayerProperties : public VkLayerProperties {
    public:
        LayerProperties() = default;

        static std::vector<LayerProperties> Enumerate();
        static bool ValidationLayerSupported(std::span<char const*> validationLayers, std::span<LayerProperties> instanceLayersProperties);

        static void DisplayLayerProperties(LayerProperties const& layerProperties);
        static void DisplayLayersProperties(std::span<LayerProperties> layersProperties);
};

#endif // VK_WRAPPER_LAYER_PROPERTIES_HPP