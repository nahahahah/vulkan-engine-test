#include "VulkanHelpers/ParameterInfos/SubmitInfo.hpp"

VkSubmitInfo2 GenerateSumbitInfo(
    std::span<VkCommandBufferSubmitInfo> commandBuffersSubmitInfo,
    std::span<VkSemaphoreSubmitInfo> signalSemaphoresSubmitInfo,
    std::span<VkSemaphoreSubmitInfo> waitSemaphoresSubmitInfo
) {
    VkSubmitInfo2 infos {};

    // structure type and flags
    infos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    infos.flags = 0;

    // command buffers
    infos.commandBufferInfoCount = static_cast<uint32_t>(commandBuffersSubmitInfo.size());
    infos.pCommandBufferInfos = commandBuffersSubmitInfo.data();

    // signal semaphores
    infos.signalSemaphoreInfoCount = static_cast<uint32_t>(signalSemaphoresSubmitInfo.size());
    infos.pSignalSemaphoreInfos = signalSemaphoresSubmitInfo.data();

    // wait semaphores
    infos.waitSemaphoreInfoCount = static_cast<uint32_t>(waitSemaphoresSubmitInfo.size());
    infos.pWaitSemaphoreInfos = waitSemaphoresSubmitInfo.data();

    // extend infos
    infos.pNext = VK_NULL_HANDLE;

    return infos;
}
