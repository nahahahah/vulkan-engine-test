#include "VulkanHelpers/PipelineColorBlendStateCreateInfo.hpp"

VkPipelineColorBlendStateCreateInfo GeneratePipelineColorBlendStateCreateInfo(std::span<VkPipelineColorBlendAttachmentState> states) {
    VkPipelineColorBlendStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // attachments
    createInfo.attachmentCount = static_cast<uint32_t>(states.size());
    createInfo.pAttachments = states.data();

    // blend properties
    createInfo.blendConstants[0] = 0.0f;
    createInfo.blendConstants[1] = 0.0f;
    createInfo.blendConstants[2] = 0.0f;
    createInfo.blendConstants[3] = 0.0f;

    // logic properties
    createInfo.logicOpEnable = VK_FALSE;
    createInfo.logicOp = VK_LOGIC_OP_COPY;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}