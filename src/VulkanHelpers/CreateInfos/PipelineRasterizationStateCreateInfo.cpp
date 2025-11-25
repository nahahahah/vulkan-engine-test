#include "VulkanHelpers/CreateInfos/PipelineRasterizationStateCreateInfo.hpp"

VkPipelineRasterizationStateCreateInfo GeneratePipelineRasterizationStateCreateInfo() {
    VkPipelineRasterizationStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // faces properties
    createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    createInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    createInfo.polygonMode = VK_POLYGON_MODE_FILL;
    createInfo.lineWidth = 1.0f;

    // depth properties
    createInfo.depthBiasEnable = VK_FALSE;
    createInfo.depthBiasConstantFactor = 0.0f;
    createInfo.depthBiasSlopeFactor = 0.0f;
    createInfo.depthClampEnable = VK_FALSE;
    createInfo.depthBiasClamp = 0.0f;
    createInfo.rasterizerDiscardEnable = VK_FALSE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
