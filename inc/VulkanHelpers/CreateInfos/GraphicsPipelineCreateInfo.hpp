#ifndef VK_WRAPPER_GRAPHICS_PIPELINE_CREATE_INFO_HPP
#define VK_WRAPPER_GRAPHICS_PIPELINE_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

#include <VulkanHelpers/Handles/PipelineLayout.hpp>
#include <VulkanHelpers/Handles/RenderPass.hpp>

VkGraphicsPipelineCreateInfo GenerateGraphicsPipelineCreateInfo(
    VkPipelineDynamicStateCreateInfo const& dynamicStateCreateInfo,
    VkPipelineVertexInputStateCreateInfo const& vertexInputStateCreateInfo,
    VkPipelineInputAssemblyStateCreateInfo const& inputAssemblyStateCreateInfo,
    VkPipelineViewportStateCreateInfo const& viewportStateCreateInfo,
    VkPipelineRasterizationStateCreateInfo const& rasterizationStateCreateInfo,
    VkPipelineMultisampleStateCreateInfo const& multisampleStateCreateInfo,
    VkPipelineDepthStencilStateCreateInfo const& depthStencilStateCreateInfo,
    VkPipelineColorBlendStateCreateInfo const& colorBlendStateCreateInfo,
    PipelineLayout const& pipelineLayout,
    std::span<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos,
    RenderPass const& renderPass
);

#endif // VK_WRAPPER_GRAPHICS_PIPELINE_CREATE_INFO_HPP
