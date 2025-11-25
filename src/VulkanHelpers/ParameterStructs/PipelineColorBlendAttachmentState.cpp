#include "VulkanHelpers/ParameterStructs/PipelineColorBlendAttachmentState.hpp"

VkPipelineColorBlendAttachmentState GeneratePipelineColorBlendAttachmentState() {
    VkPipelineColorBlendAttachmentState state {};
    
    // state properties
    state.blendEnable = VK_FALSE;
    state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    // color blend factor
    state.colorBlendOp = VK_BLEND_OP_ADD;
    state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;

    // alpha blend factor
    state.alphaBlendOp = VK_BLEND_OP_ADD;
    state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

    return state;
}
