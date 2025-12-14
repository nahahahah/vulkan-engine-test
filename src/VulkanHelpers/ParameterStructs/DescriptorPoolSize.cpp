#include "VulkanHelpers/ParameterStructs/DescriptorPoolSize.hpp"

VkDescriptorPoolSize GenerateDescriptorPoolSize(VkDescriptorType type, uint32_t descriptorCount) {
    VkDescriptorPoolSize poolSize {};

    // pool size properties
    poolSize.type = type;
    poolSize.descriptorCount = descriptorCount;

    return poolSize;
}
