#include "VulkanHelpers/ParameterStructs/CopyDescriptorSet.hpp"

VkCopyDescriptorSet GenerateCopyDescriptorSet() {
    VkCopyDescriptorSet copyDescriptorSet {};

    // structure type
    copyDescriptorSet.sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;

    // array elements properties
    copyDescriptorSet.srcArrayElement = 0;
    copyDescriptorSet.dstArrayElement = 0;

    // bindings properties
    copyDescriptorSet.srcBinding = 0;
    copyDescriptorSet.dstBinding = 0;

    copyDescriptorSet.srcSet = nullptr;
    copyDescriptorSet.dstSet = nullptr;

    // descriptor count property
    copyDescriptorSet.descriptorCount = 0;

    // extend copy descriptor set
    copyDescriptorSet.pNext = VK_NULL_HANDLE;

    return copyDescriptorSet;
}
