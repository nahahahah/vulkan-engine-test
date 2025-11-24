#ifndef VK_WRAPPER_SEMAPHORE_CREATE_INFO_HPP
#define VK_WRAPPER_SEMAPHORE_CREATE_INFO_HPP

#include <vulkan/vulkan.h>

VkSemaphoreCreateInfo GenerateSemaphoreCreateInfo(uint32_t flags = 0);

#endif // VK_WRAPPER_SEMAPHORE_CREATE_INFO_HPP
