#include "VulkanHelpers/ParameterInfos/CommandBufferBeginInfo.hpp"

VkCommandBufferBeginInfo GenerateCommandBufferBeginInfo() {
    VkCommandBufferBeginInfo beginInfo {};

    // structure type and flags
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    
    // other infos
    beginInfo.pInheritanceInfo = VK_NULL_HANDLE;

    // extend begin info
    beginInfo.pNext = VK_NULL_HANDLE;

    return beginInfo;
}
