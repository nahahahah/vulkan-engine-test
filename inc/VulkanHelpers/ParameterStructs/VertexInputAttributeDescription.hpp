#ifndef VK_WRAPPER_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_HPP
#define VK_WRAPPER_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_HPP

#include <vulkan/vulkan.h>

#include "Assets.hpp"

VkVertexInputAttributeDescription GenerateVertexInputAttributeDescription(int binding, int location, VkFormat format, uint32_t offset);

#endif // VK_WRAPPER_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_HPP
