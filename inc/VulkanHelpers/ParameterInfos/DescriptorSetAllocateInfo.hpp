#ifndef VK_WRAPPER_DESCRIPTOR_SET_ALLOCATE_INFO_HPP
#define VK_WRAPPER_DESCRIPTOR_SET_ALLOCATE_INFO_HPP

#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/DescriptorPool.hpp"
#include "VulkanHelpers/Handles/DescriptorSetLayout.hpp"

VkDescriptorSetAllocateInfo GenerateDescriptorSetAllocateInfo(DescriptorPool const& descriptorPool, std::span<VkDescriptorSetLayout> setLayouts);

#endif // VK_WRAPPER_DESCRIPTOR_SET_ALLOCATE_INFO_HPP
