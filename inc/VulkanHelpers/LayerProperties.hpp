#ifndef VK_WRAPPER_LAYER_PROPERTIES_HPP
#define VK_WRAPPER_LAYER_PROPERTIES_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <span>
#include <string>
#include <cstring>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Version.hpp"

std::vector<VkLayerProperties> EnumerateLayerProperties();
bool AreValidationLayerSupported(std::span<char const*> validationLayers, std::span<VkLayerProperties> instanceLayersProperties);
std::ostream& operator << (std::ostream& out, VkLayerProperties const& layerProperties);
std::ostream& operator << (std::ostream& out, std::span<VkLayerProperties> layersProperties);

#endif // VK_WRAPPER_LAYER_PROPERTIES_HPP
