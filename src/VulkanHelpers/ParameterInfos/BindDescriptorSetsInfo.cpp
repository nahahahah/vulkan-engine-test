#include "VulkanHelpers/ParameterInfos/BindDescriptorSetsInfo.hpp"

VkBindDescriptorSetsInfo GenerateBindDescriptorSetsInfo(
    PipelineLayout const& layout,
    VkShaderStageFlags stageFlags,
    uint32_t firstSet,
    std::span<VkDescriptorSet> descriptorSets,
    std::span<uint32_t> dynamicOffsets
) {
    VkBindDescriptorSetsInfo info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_BIND_DESCRIPTOR_SETS_INFO;

    // stage flags properties
    info.stageFlags = stageFlags;

    // descriptor sets properties
    info.firstSet = firstSet;
    info.descriptorSetCount = static_cast<uint32_t>(descriptorSets.size());
    info.pDescriptorSets = (descriptorSets.size() > 0) ? descriptorSets.data() : VK_NULL_HANDLE;

    // dynamic offset properties
    info.dynamicOffsetCount = static_cast<uint32_t>(dynamicOffsets.size());
    info.pDynamicOffsets = (dynamicOffsets.size() > 0) ? dynamicOffsets.data() : VK_NULL_HANDLE;

    // pipeline layout properties
    info.layout = layout.Handle();

    // extend bind info
    info.pNext = VK_NULL_HANDLE;

    return info;
}

