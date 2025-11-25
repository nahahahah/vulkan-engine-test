#include "VulkanHelpers/ParameterInfos/RenderPassBeginInfo.hpp"

VkRenderPassBeginInfo GenerateRenderPassBeginInfo(
    RenderPass const& renderPass,
    Framebuffer const& framebuffer,
    std::span<VkClearValue> clearValues,
    VkRect2D renderArea,
    void* next
) {
    VkRenderPassBeginInfo beginInfo {};

    // structure type
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

    // clear values
    beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    beginInfo.pClearValues = clearValues.data();

    // render pass properties
    beginInfo.renderPass = renderPass.Handle();
    beginInfo.framebuffer = framebuffer.Handle();

    // render area properties
    beginInfo.renderArea = renderArea; 

    // extend begin info
    beginInfo.pNext = next;

    return beginInfo;
}
