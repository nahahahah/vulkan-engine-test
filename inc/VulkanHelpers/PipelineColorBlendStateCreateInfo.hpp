#ifndef VK_WRAPPER_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_HPP
#define VK_WRAPPER_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkPipelineColorBlendStateCreateInfo GeneratePipelineColorBlendStateCreateInfo(std::span<VkPipelineColorBlendAttachmentState> states);

#endif // VK_WRAPPER_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_HPP
