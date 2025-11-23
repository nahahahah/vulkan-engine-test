#include "VulkanHelpers/Viewport.hpp"

VkViewport GenerateViewport(float width, float height) {
    VkViewport viewport {};

    viewport.width = width;
    viewport.height = height;

    viewport.maxDepth = 1.0f;
    viewport.minDepth = 0.0f;

    viewport.x = 0.0f;
    viewport.y = 0.0f;

    return viewport;
}
