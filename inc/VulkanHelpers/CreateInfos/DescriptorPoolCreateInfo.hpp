#ifndef VK_WRAPPER_DESCRIPTOR_POOL_CREATE_INFO_HPP
#define VK_WRAPPER_DESCRIPTOR_POOL_CREATE_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkDescriptorPoolCreateInfo GenerateDescriptorPoolCreateInfo(std::span<VkDescriptorPoolSize> poolSizes, uint32_t maxSet);

#endif // VK_WRAPPER_DESCRIPTOR_POOL_CREATE_INFO_HPP
