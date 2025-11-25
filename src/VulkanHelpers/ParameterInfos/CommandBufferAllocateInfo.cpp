#include "VulkanHelpers/ParameterInfos/CommandBufferAllocateInfo.hpp"

VkCommandBufferAllocateInfo GenerateCommandBufferAllocateInfo(CommandPool const& commandPool, uint32_t count) {
    VkCommandBufferAllocateInfo allocateInfo {};

    // structure type
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    // allocation properties
    allocateInfo.commandPool = commandPool.Handle();
    allocateInfo.commandBufferCount = count;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    // extend allocate info
    allocateInfo.pNext = VK_NULL_HANDLE;

    return allocateInfo;
}
