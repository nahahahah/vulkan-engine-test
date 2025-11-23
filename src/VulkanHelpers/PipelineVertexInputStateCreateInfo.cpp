#include "VulkanHelpers/PipelineVertexInputStateCreateInfo.hpp"

VkPipelineVertexInputStateCreateInfo GeneratePipelineVertexInputStateCreateInfo() {
    VkPipelineVertexInputStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // attribute description
    createInfo.vertexAttributeDescriptionCount = 0;
    createInfo.pVertexAttributeDescriptions = VK_NULL_HANDLE;

    // binding description
    createInfo.vertexBindingDescriptionCount = 0;
    createInfo.pVertexBindingDescriptions = VK_NULL_HANDLE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
