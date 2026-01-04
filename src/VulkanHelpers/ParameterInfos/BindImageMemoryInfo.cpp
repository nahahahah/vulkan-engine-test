#include "VulkanHelpers/ParameterInfos/BindImageMemoryInfo.hpp"

VkBindImageMemoryInfo GenerateBindImageMemoryInfo(Image const& image, DeviceMemory const& memory, VkDeviceSize memoryOffset) {
    VkBindImageMemoryInfo info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;

    // bind buffer properties
    info.image = image.Handle();
    info.memory = memory.Handle();
    info.memoryOffset = memoryOffset;

    // extend info
    info.pNext = VK_NULL_HANDLE;

    return info;
}
