#include "VulkanHelpers/ParameterStructs/ImageBlit.hpp"

VkImageBlit2 GenerateImageBlit(
    uint32_t srcMipLevel,
    VkOffset3D const& srcOffsets1,
    uint32_t dstMipLevel,
    VkOffset3D const& dstOffsets1
) {
    VkImageBlit2 imageBlit {};

    // structure type
    imageBlit.sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2;

    // source properties
    imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageBlit.srcSubresource.mipLevel = srcMipLevel;
    imageBlit.srcSubresource.layerCount = 1;
    imageBlit.srcSubresource.baseArrayLayer = 0;
    imageBlit.srcOffsets[0] = { 0, 0, 0 };
    imageBlit.srcOffsets[1] = srcOffsets1;

    // destination properties
    imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageBlit.dstSubresource.mipLevel = dstMipLevel;
    imageBlit.dstSubresource.layerCount = 1;
    imageBlit.dstSubresource.baseArrayLayer = 0;
    imageBlit.dstOffsets[0] = { 0, 0, 0 };
    imageBlit.dstOffsets[1] = dstOffsets1;

    // extend image blit
    imageBlit.pNext = VK_NULL_HANDLE;

    return imageBlit;
}
