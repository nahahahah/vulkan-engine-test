#include "VulkanHelpers/ParameterInfos/DependencyInfo.hpp"

VkDependencyInfo GenerateDependencyInfo(
    std::span<VkMemoryBarrier2> memoryBarriers,
    std::span<VkBufferMemoryBarrier2> bufferMemoryBarriers,
    std::span<VkImageMemoryBarrier2> imageMemoryBarriers
) {
    VkDependencyInfo info {};

    // structure type and flags
    info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    info.dependencyFlags = 0;

    // memory barriers properties
    info.memoryBarrierCount = static_cast<uint32_t>(memoryBarriers.size());
    info.pMemoryBarriers = memoryBarriers.data();
    
    // buffer memory barriers properties
    info.bufferMemoryBarrierCount = static_cast<uint32_t>(bufferMemoryBarriers.size());
    info.pBufferMemoryBarriers = bufferMemoryBarriers.data();

    // image memory barriers properties
    info.imageMemoryBarrierCount = static_cast<uint32_t>(imageMemoryBarriers.size());
    info.pImageMemoryBarriers = imageMemoryBarriers.data();

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
