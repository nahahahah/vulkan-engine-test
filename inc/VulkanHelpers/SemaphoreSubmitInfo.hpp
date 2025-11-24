#ifndef VK_WRAPPER_SEMAPHORE_SUBMIT_INFO_HPP
#define VK_WRAPPER_SEMAPHORE_SUBMIT_INFO_HPP

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Semaphore.hpp"

VkSemaphoreSubmitInfo GenerateSemaphoreSubmitInfo(Semaphore const& semaphore, VkPipelineStageFlags2 stageMask = 0);

#endif // VK_WRAPPER_SEMAPHORE_SUBMIT_INFO_HPP
