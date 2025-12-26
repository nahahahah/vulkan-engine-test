#ifndef VK_WRAPPER_DESCRIPTOR_IMAGE_INFO_HPP
#define VK_WRAPPER_DESCRIPTOR_IMAGE_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/ImageView.hpp"
#include "VulkanHelpers/Handles/Sampler.hpp"

VkDescriptorImageInfo GenerateDescriptorImageInfo(
    ImageView const& imageView,
    Sampler const& sampler,
    VkImageLayout layout
);

#endif // VK_WRAPPER_DESCRIPTOR_IMAGE_INFO_HPP
