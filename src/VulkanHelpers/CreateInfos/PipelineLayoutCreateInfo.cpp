#include "VulkanHelpers/CreateInfos/PipelineLayoutCreateInfo.hpp"

VkPipelineLayoutCreateInfo GeneratePipelineLayoutCreateInfo() {
    VkPipelineLayoutCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.flags = 0;

    // set layouts
    createInfo.setLayoutCount = 0;
    createInfo.pSetLayouts = VK_NULL_HANDLE;

    // push constants
    createInfo.pushConstantRangeCount = 0;
    createInfo.pPushConstantRanges = VK_NULL_HANDLE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}