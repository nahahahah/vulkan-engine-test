#ifndef VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_CREATE_INFO_HPP
#define VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_CREATE_INFO_HPP

#include <span>
#include <vector>

#include <vulkan/vulkan.h>

VkDescriptorSetLayoutCreateInfo GenerateDescriptorSetLayoutCreateInfo(std::span<VkDescriptorSetLayoutBinding> layoutBindings);

#endif // VK_WRAPPER_DESCRIPTOR_SET_LAYOUT_CREATE_INFO_HPP