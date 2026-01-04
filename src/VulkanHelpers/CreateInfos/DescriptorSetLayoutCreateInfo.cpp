#include "VulkanHelpers/CreateInfos/DescriptorSetLayoutCreateInfo.hpp"

VkDescriptorSetLayoutCreateInfo GenerateDescriptorSetLayoutCreateInfo(std::span<VkDescriptorSetLayoutBinding> layoutBindings) {
    VkDescriptorSetLayoutCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.flags = 0;

    // binding properties
    createInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    createInfo.pBindings = (layoutBindings.size() > 0) ? layoutBindings.data() : VK_NULL_HANDLE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
