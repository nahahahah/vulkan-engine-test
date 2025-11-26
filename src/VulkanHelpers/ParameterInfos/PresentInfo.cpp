#include "VulkanHelpers/ParameterInfos/PresentInfo.hpp"

VkPresentInfoKHR GeneratePresentInfo(
    std::span<uint32_t> imageIndices,
    std::span<VkSwapchainKHR> swapChains,
    std::span<VkSemaphore> waitSemaphores,
    std::span<VkResult> results,
    void* next
) {
    VkPresentInfoKHR infos {};

    // structure type
    infos.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    // image indices (probably uses the same array size as swapChains)
    infos.pImageIndices = imageIndices.data();
    
    // swapchains
    infos.swapchainCount = static_cast<uint32_t>(swapChains.size());
    infos.pSwapchains = swapChains.data();

    // results (same comment as image indices regarding the size of this array)
    infos.pResults = results.data();

    // wait semaphores
    infos.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    infos.pWaitSemaphores = waitSemaphores.data();

    // extend present info
    infos.pNext = next;

    return infos;
}
