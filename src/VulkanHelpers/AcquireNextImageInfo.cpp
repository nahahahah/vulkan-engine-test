#include "VulkanHelpers/AcquireNextImageInfo.hpp"

VkAcquireNextImageInfoKHR GenerateAcquireNextImageInfo(
    Swapchain const& swapChain,
    Semaphore* semaphore,
    Fence* fence,
    uint32_t deviceMask,
    void* next,
    uint64_t timeout
) {
    VkAcquireNextImageInfoKHR info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;

    // image properties
    info.deviceMask = deviceMask;
    info.swapchain = swapChain.Handle();
    info.timeout = timeout;

    // synchronization properties
    info.fence = (fence != nullptr ? fence->Handle() : VK_NULL_HANDLE);
    info.semaphore = (semaphore != nullptr ? semaphore->Handle() : VK_NULL_HANDLE);

    // extend info
    info.pNext = next;

    return info;
}
