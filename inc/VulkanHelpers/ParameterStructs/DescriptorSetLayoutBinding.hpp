#ifndef VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_BINDING_HPP
#define VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_BINDING_HPP

#include <vulkan/vulkan.h>

VkDescriptorSetLayoutBinding GenerateDescriptorSetLayoutBinding(
    uint32_t binding,
    VkDescriptorType descriptorType,
    uint32_t descriptorCount,
    VkShaderStageFlags stateFlags
);

#endif // VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_BINDING_HPP