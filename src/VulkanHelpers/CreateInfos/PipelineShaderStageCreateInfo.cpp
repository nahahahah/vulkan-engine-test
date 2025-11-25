#include "VulkanHelpers/CreateInfos/PipelineShaderStageCreateInfo.hpp"

VkPipelineShaderStageCreateInfo GeneratePipelineShaderStageCreateInfo(
    ShaderModule const& shaderModule,
    std::string const& mainFunctionName,
    VkShaderStageFlagBits stage
) {
    VkPipelineShaderStageCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.flags = 0;

    // shader info
    createInfo.stage = stage;
    createInfo.pName = mainFunctionName.c_str();
    createInfo.module = shaderModule.Handle();
    createInfo.pSpecializationInfo = VK_NULL_HANDLE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
