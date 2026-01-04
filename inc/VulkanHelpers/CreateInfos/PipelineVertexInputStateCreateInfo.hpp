#ifndef VK_WRAPPER_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_HPP
#define VK_WRAPPER_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkPipelineVertexInputStateCreateInfo GeneratePipelineVertexInputStateCreateInfo(
    std::span<VkVertexInputBindingDescription> bindingDescriptions,
    std::span<VkVertexInputAttributeDescription> bindingAttributes
);

#endif // VK_WRAPPER_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_HPP
