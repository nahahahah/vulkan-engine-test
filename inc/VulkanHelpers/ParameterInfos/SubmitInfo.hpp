#ifndef VK_WRAPPER_SUBMIT_INFO
#define VK_WRAPPER_SUBMIT_INFO

#include <span>

#include <vulkan/vulkan.h>

VkSubmitInfo2 GenerateSumbitInfo(
    std::span<VkCommandBufferSubmitInfo> commandBuffersSubmitInfo,
    std::span<VkSemaphoreSubmitInfo> signalSemaphoresSubmitInfo,
    std::span<VkSemaphoreSubmitInfo> waitSemaphoresSubmitInfo
);

#endif // VK_WRAPPER_SUBMIT_INFO
