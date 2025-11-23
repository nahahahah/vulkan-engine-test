#include "VulkanHelpers/PipelineDepthStencilStateCreateInfo.hpp"

VkPipelineDepthStencilStateCreateInfo GeneratePipelineDepthStencilStateCreateInfo() {
    VkPipelineDepthStencilStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // stencil properties
    createInfo.stencilTestEnable = VK_FALSE;
    createInfo.back = VkStencilOpState {};
    createInfo.front = VkStencilOpState {};

    // depth properties
    createInfo.depthTestEnable = VK_FALSE;
    createInfo.depthWriteEnable = VK_FALSE;
    createInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS;

    // depth bounds range
    createInfo.depthBoundsTestEnable = VK_FALSE;
    createInfo.maxDepthBounds = 1.0f;
    createInfo.minDepthBounds = 0.0f;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
