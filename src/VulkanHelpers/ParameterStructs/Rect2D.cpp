#include "VulkanHelpers/ParameterStructs/Rect2D.hpp"

VkRect2D GenerateRect2D(VkExtent2D extent) {
    VkRect2D scissor {};

    scissor.extent = extent;
    scissor.offset = VkOffset2D { .x = 0, .y = 0 };

    return scissor;
}
