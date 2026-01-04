#include "VulkanHelpers/ParameterInfos/DescriptorBufferInfo.hpp"

VkDescriptorBufferInfo GenerateDescriptorBufferInfo(Buffer const& buffer, VkDeviceSize range) {
    VkDescriptorBufferInfo info {};

    // descriptor buffer properties
    info.buffer = buffer.Handle();
    info.offset = 0;
    info.range = range;

    return info;
}
