#include "VulkanHelpers/CreateInfos/PipelineLayoutCreateInfo.hpp"

VkPipelineLayoutCreateInfo GeneratePipelineLayoutCreateInfo(
    std::span<VkDescriptorSetLayout> setLayouts,
    std::span<VkPushConstantRange> pushConstants
) {
    VkPipelineLayoutCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.flags = 0;

    // set layouts
    createInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
    createInfo.pSetLayouts = (setLayouts.size() > 0) ? setLayouts.data() : VK_NULL_HANDLE;

    // push constants
    createInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstants.size());
    createInfo.pPushConstantRanges = (pushConstants.size() > 0) ? pushConstants.data() : VK_NULL_HANDLE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}