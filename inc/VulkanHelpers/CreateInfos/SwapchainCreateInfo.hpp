#ifndef VK_WRAPPER_SWAPCHAIN_CREATE_INFO_HPP
#define VK_WRAPPER_SWAPCHAIN_CREATE_INFO_HPP

#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Surface.hpp"

VkSwapchainCreateInfoKHR GenerateSwapchainCreateInfo(
    VkColorSpaceKHR imageColorSpace,
    VkExtent2D extent,
    VkFormat imageFormat,
    uint32_t graphicsQueueFamilyIndex,
    uint32_t presentQueueFamilyIndex,
    uint32_t imageCount,
    VkPresentModeKHR presentMode,
    VkSurfaceTransformFlagBitsKHR surfaceTransform,
    Surface const& surface
);

#endif // VK_WRAPPER_SWAPCHAIN_CREATE_INFO_HPP
