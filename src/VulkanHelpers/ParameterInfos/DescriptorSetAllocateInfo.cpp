#include "VulkanHelpers/ParameterInfos/DescriptorSetAllocateInfo.hpp"

VkDescriptorSetAllocateInfo GenerateDescriptorSetAllocateInfo(DescriptorPool const& descriptorPool, std::span<VkDescriptorSetLayout> setLayouts) {
    VkDescriptorSetAllocateInfo info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

    // descriptor sets properties
    info.descriptorSetCount = static_cast<uint32_t>(setLayouts.size());
    info.pSetLayouts = (setLayouts.size() > 0) ? setLayouts.data() : VK_NULL_HANDLE;

    // descriptor pool
    info.descriptorPool = descriptorPool.Handle();

    // extend allocate info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
