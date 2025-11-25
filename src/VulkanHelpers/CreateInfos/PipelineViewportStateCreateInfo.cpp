#include "VulkanHelpers/CreateInfos/PipelineViewportStateCreateInfo.hpp"

VkPipelineViewportStateCreateInfo GeneratePipelineViewportStateCreateInfo(std::span<VkViewport> viewports, std::span<VkRect2D> scissors) {
    VkPipelineViewportStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // viewport properties 
    createInfo.viewportCount = static_cast<uint32_t>(viewports.size());
    createInfo.pViewports = viewports.data();
    
    // scissor properties
    createInfo.scissorCount = static_cast<uint32_t>(scissors.size());
    createInfo.pScissors = scissors.data();

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
