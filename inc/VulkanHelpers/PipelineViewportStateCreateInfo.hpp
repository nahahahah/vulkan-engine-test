#ifndef VK_WRAPPER_PIPELINE_VIEWPORT_STATE_CREATE_INFO_HPP
#define VK_WRAPPER_PIPELINE_VIEWPORT_STATE_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkPipelineViewportStateCreateInfo GeneratePipelineViewportStateCreateInfo(std::span<VkViewport> viewports, std::span<VkRect2D> scissors);

#endif // VK_WRAPPER_VIEWPORT_STATE_CREATE_INFO_HPP
