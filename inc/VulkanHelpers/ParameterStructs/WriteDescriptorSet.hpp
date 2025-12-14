#ifndef VK_WRAPPER_WRITE_DESCRIPTOR_SET_HPP
#define VK_WRAPPER_WRITE_DESCRIPTOR_SET_HPP

#include <span>

#include <vulkan/vulkan.h>

VkWriteDescriptorSet GenerateWriteDescriptorSet(
    uint32_t descriptorCount,
    VkDescriptorType descriptorType, 
    VkDescriptorSet dstSet,
    uint32_t dstBinding,
    uint32_t dstArrayElement = 0,
    VkDescriptorBufferInfo* bufferInfo = nullptr,
    VkDescriptorImageInfo* imageInfo = nullptr
);

#endif // VK_WRAPPER_WRITE_DESCRIPTOR_SET_HPP
