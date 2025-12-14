#include "VulkanHelpers/ParameterStructs/DescriptorSetLayoutBinding.hpp"

VkDescriptorSetLayoutBinding GenerateDescriptorSetLayoutBinding(
    uint32_t binding,
    VkDescriptorType descriptorType,
    uint32_t descriptorCount,
    VkShaderStageFlags stateFlags
) {
    VkDescriptorSetLayoutBinding layoutBinding {};

    // binding properties
    layoutBinding.binding = binding;

    // descriptor properties
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = descriptorCount;
    
    // stage properties
    layoutBinding.stageFlags = stateFlags;

    // samplers properties
    layoutBinding.pImmutableSamplers = VK_NULL_HANDLE;

    return layoutBinding;
}
