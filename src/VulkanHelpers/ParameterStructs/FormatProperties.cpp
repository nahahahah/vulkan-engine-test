#include "VulkanHelpers/ParameterStructs/FormatProperties.hpp"

VkFormatProperties2 GenerateFormatProperties() {
    VkFormatProperties2 formatProperties {};

    // structure type
    formatProperties.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
    
    // format properties
    formatProperties.formatProperties = {};

    // extend format properties
    formatProperties.pNext = VK_NULL_HANDLE;

    return formatProperties;
}
