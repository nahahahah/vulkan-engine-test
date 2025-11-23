#include "VulkanHelpers/GraphicsPipelineCreateInfo.hpp"

VkGraphicsPipelineCreateInfo GenerateGraphicsPipelineCreateInfo(
    VkPipelineDynamicStateCreateInfo const& dynamicStateCreateInfo,
    VkPipelineVertexInputStateCreateInfo const& vertexInputStateCreateInfo,
    VkPipelineInputAssemblyStateCreateInfo const& inputAssemblyStateCreateInfo,
    VkPipelineViewportStateCreateInfo const& viewportStateCreateInfo,
    VkPipelineRasterizationStateCreateInfo const& rasterizationStateCreateInfo,
    VkPipelineMultisampleStateCreateInfo const& multisampleStateCreateInfo,
    VkPipelineColorBlendStateCreateInfo const& colorBlendStateCreateInfo,
    PipelineLayout const& pipelineLayout,
    std::span<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos,
    RenderPass const& renderPass
) {
    VkGraphicsPipelineCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.flags = 0;

    // states
    createInfo.pDynamicState = &dynamicStateCreateInfo;
    createInfo.pVertexInputState = &vertexInputStateCreateInfo;
    createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
    createInfo.pViewportState = &viewportStateCreateInfo;
    createInfo.pRasterizationState = &rasterizationStateCreateInfo;
    createInfo.pTessellationState = VK_NULL_HANDLE;
    createInfo.pMultisampleState = &multisampleStateCreateInfo;
    createInfo.pDepthStencilState = VK_NULL_HANDLE;
    createInfo.pColorBlendState = &colorBlendStateCreateInfo;

    // base pipeline
    createInfo.basePipelineIndex = -1;
    createInfo.basePipelineHandle = VK_NULL_HANDLE;

    // layout
    createInfo.layout = pipelineLayout.Handle();

    // stages
    createInfo.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size());
    createInfo.pStages = shaderStageCreateInfos.data();

    // render pass and subpass
    createInfo.renderPass = renderPass.Handle();
    createInfo.subpass = 0;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}