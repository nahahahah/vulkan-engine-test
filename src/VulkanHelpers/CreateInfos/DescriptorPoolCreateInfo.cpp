#include "VulkanHelpers/CreateInfos/DescriptorPoolCreateInfo.hpp"

VkDescriptorPoolCreateInfo GenerateDescriptorPoolCreateInfo(std::span<VkDescriptorPoolSize> poolSizes, uint32_t maxSet) {
    VkDescriptorPoolCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.flags = 0;

    // pool sizes properties
    createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    createInfo.pPoolSizes = (poolSizes.size() > 0) ? poolSizes.data() : VK_NULL_HANDLE;

    // max sets properties
    createInfo.maxSets = maxSet;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
