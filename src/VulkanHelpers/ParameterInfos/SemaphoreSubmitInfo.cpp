#include "VulkanHelpers/ParameterInfos/SemaphoreSubmitInfo.hpp"

VkSemaphoreSubmitInfo GenerateSemaphoreSubmitInfo(Semaphore const& semaphore, VkPipelineStageFlags2 stageMask) {
    VkSemaphoreSubmitInfo submitInfo {};

    // structure type
    submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;

    // semaphore handle
    submitInfo.semaphore = semaphore.Handle();

    // other submit properties
    submitInfo.deviceIndex = 0;
    submitInfo.stageMask = stageMask;
    submitInfo.value = 0;

    // extend submit info
    submitInfo.pNext = VK_NULL_HANDLE;

    return submitInfo;
}
