#include "Application.hpp"

// debug callback
#ifndef NDEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* pUserData
) {
    (void)(pUserData);

    std::clog << "\n[ ";
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            std::clog << "VERBOSE";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            std::clog << "INFO";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            std::clog << "WARNING";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            std::clog << "ERROR";
            break;
        }

        default: {
            break;
        }
    }

    std::clog << "::";
    
    switch (messageType) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
            std::clog << "GENERAL"; 
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
            std::clog << "VALIDATION";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
            std::clog << "PERFORMANCE";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: {
            std::clog << "DEVICE ADRESS BINDING";
            break; 
        }

        default: {
            break;
        }
    }

    std::cout << " ] ";

    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl << std::endl;
    return VK_FALSE;
}
#endif

/// TODO: VERY BAD IDEA HERE. Find a way to NOT give access to the handle (especially giving the right to create/destroy it)
void HandleFrameInvalidity(
    PhysicalDevice const& physicalDevice,
    Device& device,
    Surface const& surface,
    VkSwapchainCreateInfoKHR const& swapchainCreateInfo,
    Swapchain& swapchain,
    VkFormat imageFormat,
    std::vector<VkImage>& swapchainImages,
    std::vector<ImageView>& swapchainImageViews,
    std::vector<Framebuffer>& framebuffers,
    RenderPass const& renderPass
) {
    device.WaitIdle();

    for (auto& framebuffer : framebuffers) {
        framebuffer.DestroyHandle();
    }

    for (auto& swapchainImageView : swapchainImageViews) {
        swapchainImageView.DestroyHandle();
    }

    swapchainImages.clear();

    swapchain.DestroyHandle();

    std::cout << swapchainCreateInfo.imageExtent.width << "x" << swapchainCreateInfo.imageExtent.height << std::endl;

    auto physicalDeviceSurfaceInfo = GeneratePhysicalDeviceSurfaceInfo(surface); // get surface info
    physicalDevice.SurfaceCapabilities(physicalDeviceSurfaceInfo);
    physicalDevice.SurfaceFormats(physicalDeviceSurfaceInfo); // get formats of the surface
    physicalDevice.PresentModes(surface); // get present modes of the surface

    swapchain.CreateHandle(swapchainCreateInfo);

    swapchainImages = EnumerateSwapChainImages(device, swapchain);

    for (int i = 0; i < static_cast<int>(swapchainImages.size()); ++i) {
        auto swapchainImageViewCreateInfo = GenerateImageViewCreateInfo(imageFormat, swapchainImages[i]);
        swapchainImageViews[i].CreateHandle(swapchainImageViewCreateInfo);
    }

    for (int i = 0; i < static_cast<int>(swapchainImageViews.size()); ++i) {
        std::vector<VkImageView> attachments = { swapchainImageViews[i].Handle() };
        auto frameBufferCreateInfo = GenerateFramebufferCreateInfo(swapchainCreateInfo.imageExtent, attachments, renderPass);
        framebuffers[i].CreateHandle(frameBufferCreateInfo);
    }
}
