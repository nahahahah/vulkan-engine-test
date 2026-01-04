#ifndef VK_WRAPPER_DESCRIPTOR_POOL_SIZE_HPP
#define VK_WRAPPER_DESCRIPTOR_POOL_SIZE_HPP

#include <vulkan/vulkan.h>

VkDescriptorPoolSize GenerateDescriptorPoolSize(VkDescriptorType type, uint32_t descriptorCount);

#endif // VK_WRAPPER_DESCRIPTOR_POOL_SIZE_HPP
