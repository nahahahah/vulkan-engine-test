#ifndef VK_WRAPPER_IMAGE_BLIT_HPP
#define VK_WRAPPER_IMAGE_BLIT_HPP

#include <vulkan/vulkan.h>

VkImageBlit2 GenerateImageBlit(
    uint32_t srcMipLevel,
    VkOffset3D const& srcOffsets1,
    uint32_t dstMipLevel,
    VkOffset3D const& dstOffsets1
);

#endif //  VK_WRAPPER_IMAGE_BLIT_HPP
