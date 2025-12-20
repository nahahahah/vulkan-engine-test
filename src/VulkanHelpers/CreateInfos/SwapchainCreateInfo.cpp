#include "VulkanHelpers/CreateInfos/SwapchainCreateInfo.hpp"

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
) {
    VkSwapchainCreateInfoKHR createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.flags = 0;

    createInfo.clipped = VK_TRUE;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.imageArrayLayers = 1;
    createInfo.imageColorSpace = imageColorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageFormat = imageFormat;
    
    std::vector<uint32_t> queueFamilyIndices = {
        graphicsQueueFamilyIndex,
        presentQueueFamilyIndex
    };

    if (graphicsQueueFamilyIndex != presentQueueFamilyIndex) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }

    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = VK_NULL_HANDLE;
    }

    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.minImageCount = imageCount;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.presentMode = presentMode;
    createInfo.preTransform = surfaceTransform;
    createInfo.surface = surface.Handle();

    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
