#ifndef VK_WRAPPER_PIPELINE_STATE_CREATE_INFO_HPP
#define VK_WRAPPER_PIPELINE_STATE_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkPipelineDynamicStateCreateInfo GeneratePipelineDynamicStateCreateInfo(std::span<VkDynamicState> dynamicStates);

#endif // VK_WRAPPER_PIPELINE_STATE_CREATE_INFO_HPP
