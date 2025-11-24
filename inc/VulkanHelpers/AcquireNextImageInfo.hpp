#ifndef VK_WRAPPER_ACQUIRE_NEXT_IMAGE_INFO_HPP
#define VK_WRAPPER_ACQUIRE_NEXT_IMAGE_INFO_HPP

#include <limits>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Swapchain.hpp"
#include "VulkanHelpers/Semaphore.hpp"
#include "VulkanHelpers/Fence.hpp"

VkAcquireNextImageInfoKHR GenerateAcquireNextImageInfo(
    Swapchain const& swapChain,
    Semaphore* semaphore = nullptr,
    Fence* fence = nullptr,
    uint32_t deviceMask = 1,
    void* next = VK_NULL_HANDLE,
    uint64_t timeout = UINT64_MAX
);

#endif // VK_WRAPPER_ACQUIRE_NEXT_IMAGE_INFO_HPP
