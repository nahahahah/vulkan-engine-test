#include "VulkanHelpers/ParameterInfos/CopyBufferInfo.hpp"

VkCopyBufferInfo2 GenerateCopyBufferInfo(Buffer const& src, Buffer const& dst, std::span<VkBufferCopy2> regions) {
    VkCopyBufferInfo2 info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2;

    // buffers
    info.srcBuffer = src.Handle();
    info.dstBuffer = dst.Handle();
    
    // regions
    info.regionCount = static_cast<uint32_t>(regions.size());
    info.pRegions = (regions.size() > 0) ? regions.data() : VK_NULL_HANDLE;

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
