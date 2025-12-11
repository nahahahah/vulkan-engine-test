#include "VulkanHelpers/CreateInfos/PipelineVertexInputStateCreateInfo.hpp"

VkPipelineVertexInputStateCreateInfo GeneratePipelineVertexInputStateCreateInfo(
    std::span<VkVertexInputBindingDescription> bindingDescriptions,
    std::span<VkVertexInputAttributeDescription> bindingAttributes
) {
    VkPipelineVertexInputStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // binding description
    createInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    createInfo.pVertexBindingDescriptions = bindingDescriptions.data();

    // attribute description
    createInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(bindingAttributes.size());
    createInfo.pVertexAttributeDescriptions = bindingAttributes.data();

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
