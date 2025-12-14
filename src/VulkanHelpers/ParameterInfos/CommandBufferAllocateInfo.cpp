#include "VulkanHelpers/ParameterInfos/CommandBufferAllocateInfo.hpp"

/**
 * @brief Generates a VkCommandBufferAllocateInfo struct (for a single buffer) which is pre-filled where it can be (example sType and pNext).
 * @param commandPool The command pool with which command buffers will be allocated
 * @returns Pre-filled VkCommandBufferAllocateInfo
 */
VkCommandBufferAllocateInfo GenerateCommandBufferAllocateInfo(CommandPool const& commandPool) {
    VkCommandBufferAllocateInfo allocateInfo {};

    // structure type
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    // allocation properties
    allocateInfo.commandPool = commandPool.Handle();
    allocateInfo.commandBufferCount = 1;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    // extend allocate info
    allocateInfo.pNext = VK_NULL_HANDLE;

    return allocateInfo;
}


/**
 * @brief Generates a VkCommandBufferAllocateInfo struct (for `count` buffers) which is pre-filled where it can be (example sType and pNext). 
 * @param commandPool The command pool with which command buffers will be allocated
 * @param count Number of buffers to allocate from the command pool
 * @returns Pre-filled VkCommandBufferAllocateInfo
 */
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
