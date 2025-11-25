#ifndef VK_WRAPPER_PIPELINE_SHADER_STAGE_CREATE_INFO_HPP
#define VK_WRAPPER_PIPELINE_SHADER_STAGE_CREATE_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/ShaderModule.hpp"

VkPipelineShaderStageCreateInfo GeneratePipelineShaderStageCreateInfo(
    ShaderModule const& shaderModule,
    std::string const& mainFunctionName,
    VkShaderStageFlagBits stage
);

#endif // VK_WRAPPER_PIPELINE_SHADER_STAGE_CREATE_INFO_HPP
