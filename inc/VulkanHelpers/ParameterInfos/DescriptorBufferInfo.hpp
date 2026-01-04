#ifndef VK_WRAPPER_DESCRIPTOR_BUFFER_INFO_HPP
#define VK_WRAPPER_DESCRIPTOR_BUFFER_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Buffer.hpp"

VkDescriptorBufferInfo GenerateDescriptorBufferInfo(Buffer const& buffer, VkDeviceSize range);

#endif // VK_WRAPPER_DESCRIPTOR_BUFFER_INFO_HPP
