#include "VulkanHelpers/ParameterStructs/WriteDescriptorSet.hpp"

VkWriteDescriptorSet GenerateWriteDescriptorSet(
    uint32_t descriptorCount,
    VkDescriptorType descriptorType, 
    VkDescriptorSet dstSet,
    uint32_t dstBinding,
    uint32_t dstArrayElement,
    VkDescriptorBufferInfo* bufferInfo,
    VkDescriptorImageInfo* imageInfo
) {
    VkWriteDescriptorSet writeDescriptorSet {};
    
    // structure type
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    
    // descriptor properties
    writeDescriptorSet.descriptorCount = descriptorCount;
    writeDescriptorSet.descriptorType = descriptorType;

    // indices
    writeDescriptorSet.dstBinding = dstBinding;
    writeDescriptorSet.dstArrayElement = dstArrayElement;

    // descriptor info properties
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.pBufferInfo = (bufferInfo != nullptr) ? bufferInfo : VK_NULL_HANDLE;
    writeDescriptorSet.pImageInfo = (imageInfo != nullptr) ? imageInfo : VK_NULL_HANDLE;

    // texel buffer view properties
    writeDescriptorSet.pTexelBufferView = VK_NULL_HANDLE;

    // extend write descriptor set
    writeDescriptorSet.pNext = VK_NULL_HANDLE;

    return writeDescriptorSet;
}
