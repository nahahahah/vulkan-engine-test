#include "VulkanHelpers/ParameterInfos/DescriptorImageInfo.hpp"

VkDescriptorImageInfo GenerateDescriptorImageInfo(
    ImageView const& imageView,
    Sampler const& sampler,
    VkImageLayout layout
) {
    VkDescriptorImageInfo info {};

    // descriptor properties
    info.imageView = imageView.Handle();
    info.sampler = sampler.Handle();
    info.imageLayout = layout;

    return info;
}
