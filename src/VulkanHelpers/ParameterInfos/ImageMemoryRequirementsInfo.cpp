#include "VulkanHelpers/ParameterInfos/ImageMemoryRequirementsInfo.hpp"

VkImageMemoryRequirementsInfo2 GenerateImageMemoryRequirementsInfo(Image const& image) {
    VkImageMemoryRequirementsInfo2 info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;

    // image
    info.image = image.Handle();

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
