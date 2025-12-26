#include "VulkanHelpers/CreateInfos/SamplerCreateInfo.hpp"

VkSamplerCreateInfo GenerateSamplerCreateInfo() {
    VkSamplerCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    createInfo.flags = 0;

    // addressing modes properties
    createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    // filters properties
    createInfo.minFilter = VK_FILTER_LINEAR;
    createInfo.magFilter = VK_FILTER_LINEAR;

    // mip properties
    createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    createInfo.mipLodBias = 0.0f;
    createInfo.minLod = 0.0f;
    createInfo.maxLod = 0.0f;

    // compare properties
    createInfo.compareEnable = VK_FALSE;
    createInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    // anisotropy properties
    createInfo.anisotropyEnable = VK_FALSE;
    createInfo.maxAnisotropy = 0.0f;

    // border color properties
    createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

    // unnormalized coordinates properties
    createInfo.unnormalizedCoordinates = VK_FALSE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
