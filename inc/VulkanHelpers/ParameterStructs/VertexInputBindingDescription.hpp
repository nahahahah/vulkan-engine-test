#ifndef VK_WRAPPER_VERTEX_INPUT_BINDING_DESCRIPTION_HPP
#define VK_WRAPPER_VERTEX_INPUT_BINDING_DESCRIPTION_HPP

#include <vulkan/vulkan.h>

#include "Assets.hpp"

VkVertexInputBindingDescription GenerateVertexInputBindingDescription(uint32_t binding);

#endif // VK_WRAPPER_VERTEX_INPUT_BINDING_DESCRIPTION_HPP