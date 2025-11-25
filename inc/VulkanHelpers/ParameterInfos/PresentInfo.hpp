#ifndef VK_WRAPPER_PRESENT_INFO_HPP
#define VK_WRAPPER_PRESENT_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

VkPresentInfoKHR GeneratePresentInfo(
    std::span<uint32_t> imageIndices,
    std::span<VkSwapchainKHR> swapChains,
    std::span<VkSemaphore> waitSemaphores,
    std::span<VkResult> results = {},
    void* next = VK_NULL_HANDLE
);

#endif // VK_WRAPPER_PRESENT_INFO_HPP
