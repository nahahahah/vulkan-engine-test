#include "VulkanHelpers/ParameterStructs/VertexInputBindingDescription.hpp"

VkVertexInputBindingDescription GenerateVertexInputBindingDescription(uint32_t binding) {
    VkVertexInputBindingDescription description {};

    description.binding = binding;
    description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    description.stride = sizeof(Vertex);

    return description;
}
