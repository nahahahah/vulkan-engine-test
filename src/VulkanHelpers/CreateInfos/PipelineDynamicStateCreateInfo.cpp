#include "VulkanHelpers/CreateInfos/PipelineDynamicStateCreateInfo.hpp"

VkPipelineDynamicStateCreateInfo GeneratePipelineDynamicStateCreateInfo(std::span<VkDynamicState> dynamicStates) {
    VkPipelineDynamicStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // dynamic states
    createInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    createInfo.pDynamicStates = dynamicStates.data();

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
