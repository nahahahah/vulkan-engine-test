#include "VulkanHelpers/CreateInfos/PipelineMultisampleStateCreateInfo.hpp"

VkPipelineMultisampleStateCreateInfo GeneratePipelineMultisampleStateCreateInfo() {
    VkPipelineMultisampleStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // alpha properties
    createInfo.alphaToCoverageEnable = VK_FALSE;
    createInfo.alphaToOneEnable = VK_FALSE;

    // sample properties
    createInfo.minSampleShading = 1.0f;
    createInfo.sampleShadingEnable = VK_FALSE;
    createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.pSampleMask = VK_NULL_HANDLE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
