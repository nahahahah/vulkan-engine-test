#ifndef VK_WRAPPER_PIPELINE_LAYOUT_CREATE_INFO_HPP
#define VK_WRAPPER_PIPELINE_LAYOUT_CREATE_INFO_HPP

#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/DescriptorSetLayout.hpp"

VkPipelineLayoutCreateInfo GeneratePipelineLayoutCreateInfo(
    std::span<VkDescriptorSetLayout> setLayouts,
    std::span<VkPushConstantRange> pushConstants
);

#endif // VK_WRAPPER_PIPELINE_LAYOUT_CREATE_INFO_HPP
