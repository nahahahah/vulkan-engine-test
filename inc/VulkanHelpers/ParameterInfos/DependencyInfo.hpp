#ifndef VK_WRAPPER_DEPENDENCY_INFO_HPP
#define VK_WRAPPER_DEPENDENCY_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkDependencyInfo GenerateDependencyInfo(
    std::span<VkMemoryBarrier2> memoryBarriers,
    std::span<VkBufferMemoryBarrier2> bufferMemoryBarriers,
    std::span<VkImageMemoryBarrier2> imageMemoryBarriers
);

#endif // VK_WRAPPER_DEPENDENCY_INFO_HPP
