#include "VulkanHelpers/ParameterStructs/ClearValue.hpp"

VkClearValue GenerateClearValue() {
    VkClearValue clearValue {};

    // clear color
    clearValue.color.float32[0] = 0.0f;
    clearValue.color.float32[1] = 0.0f;
    clearValue.color.float32[2] = 0.0f;
    clearValue.color.float32[3] = 1.0f;

    // depth clear value
    clearValue.depthStencil.depth = 0.0f;

    // stencil clear value
    clearValue.depthStencil.stencil = 0;

    return clearValue;
}
