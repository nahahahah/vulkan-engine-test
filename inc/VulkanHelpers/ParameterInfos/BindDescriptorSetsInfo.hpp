#ifndef VK_WRAPPER_BIND_DESCRIPTOR_SETS_INFO_HPP
#define VK_WRAPPER_BIND_DESCRIPTOR_SETS_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PipelineLayout.hpp"

VkBindDescriptorSetsInfo GenerateBindDescriptorSetsInfo(
    PipelineLayout const& layout,
    VkShaderStageFlags stageFlags,
    uint32_t firstSet,
    std::span<VkDescriptorSet> descriptorSets,
    std::span<uint32_t> dynamicOffsets
);

#endif // VK_WRAPPER_BIND_DESCRIPTOR_SETS_INFO_HPP
