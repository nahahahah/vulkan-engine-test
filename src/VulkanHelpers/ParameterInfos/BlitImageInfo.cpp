#include "VulkanHelpers/ParameterInfos/BlitImageInfo.hpp"

VkBlitImageInfo2 GenerateBlitImageInfo(
    Image const& srcImage,
    VkImageLayout srcImageLayout,
    Image const& dstImage,
    VkImageLayout dstImageLayout,
    std::span<VkImageBlit2> regions,
    VkFilter filter
) {
    VkBlitImageInfo2 info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2;

    // source image properties
    info.srcImage = srcImage.Handle();
    info.srcImageLayout = srcImageLayout;

    // destination image properties
    info.dstImage = dstImage.Handle();
    info.dstImageLayout = dstImageLayout;

    // regions properties
    info.regionCount = static_cast<uint32_t>(regions.size());
    info.pRegions = ((regions.size() > 0) ? (regions.data()) : (VK_NULL_HANDLE));

    // filter properties
    info.filter = filter;

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
