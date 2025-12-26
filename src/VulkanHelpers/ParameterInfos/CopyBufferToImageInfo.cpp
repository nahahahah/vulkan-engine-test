#include "VulkanHelpers/ParameterInfos/CopyBufferToImageInfo.hpp"

VkCopyBufferToImageInfo2 GenerateCopyBufferToImageInfo(
    Buffer const& buffer,
    Image const& image,
    VkImageLayout dstImageLayout,
    std::span<VkBufferImageCopy2> regions
) {
    VkCopyBufferToImageInfo2 info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2;

    // copy properties
    info.srcBuffer = buffer.Handle();
    info.dstImageLayout = dstImageLayout;
    info.dstImage = image.Handle();

    // regions properties
    info.regionCount = static_cast<uint32_t>(regions.size());
    info.pRegions = regions.data();

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
