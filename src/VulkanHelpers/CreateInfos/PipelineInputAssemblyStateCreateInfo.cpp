#include "VulkanHelpers/CreateInfos/PipelineInputAssemblyStateCreateInfo.hpp"

VkPipelineInputAssemblyStateCreateInfo GeneratePipelineInputAssemblyStateCreateInfo() {
    VkPipelineInputAssemblyStateCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    createInfo.flags = 0;

    // primitive properties
    createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    createInfo.primitiveRestartEnable = VK_FALSE;

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
