#include "VulkanHelpers/ParameterInfos/CommandBufferSubmitInfo.hpp"

VkCommandBufferSubmitInfo GenerateCommandBufferSubmitInfo(CommandBuffer const& commandBuffer) {
    VkCommandBufferSubmitInfo submitInfo {};

    // structure type
    submitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;

    // command buffer properties
    submitInfo.commandBuffer = commandBuffer.Handle();
    submitInfo.deviceMask = 0;
    
    // extend submit info
    submitInfo.pNext = VK_NULL_HANDLE;

    return submitInfo;
}
