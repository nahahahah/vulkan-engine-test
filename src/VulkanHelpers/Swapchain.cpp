#include "VulkanHelpers/Swapchain.hpp"

Swapchain::Swapchain(VkSwapchainCreateInfoKHR createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateSwapchainKHR(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a swapChain (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Swap chain created successully: <VkSwapchainKHR " << _handle << ">" << std::endl;
}

Swapchain::~Swapchain() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Swap chain destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

VkExtent2D Swapchain::Extent2DFromSDLWindow(Window const& window, VkSurfaceCapabilities2KHR const& surfaceCapabilities) {   
    if (surfaceCapabilities.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return surfaceCapabilities.surfaceCapabilities.currentExtent;
    }

    else {
        int width = 0;
        int height = 0;
        if (!SDL_GetWindowSizeInPixels(window.Handle(), &width, &height)) {
            std::cerr << "Couldn't get window size: " << SDL_GetError() << std::endl;
            return surfaceCapabilities.surfaceCapabilities.currentExtent;
        }

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width  = std::clamp(actualExtent.width,
                                         surfaceCapabilities.surfaceCapabilities.minImageExtent.width,
                                         surfaceCapabilities.surfaceCapabilities.maxImageExtent.width);

        actualExtent.height = std::clamp(actualExtent.height,
                                         surfaceCapabilities.surfaceCapabilities.minImageExtent.height,
                                         surfaceCapabilities.surfaceCapabilities.maxImageExtent.height);
        
        return actualExtent;
    }
}
