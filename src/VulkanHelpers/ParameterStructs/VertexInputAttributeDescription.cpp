#include "VulkanHelpers/ParameterStructs/VertexInputAttributeDescription.hpp"

VkVertexInputAttributeDescription GenerateVertexInputAttributeDescription(int binding, int location, VkFormat format, uint32_t offset) {
    VkVertexInputAttributeDescription description {};

    description.binding = binding;
    description.location = location;
    description.format = format;
    description.offset = offset;

    return description;
}
