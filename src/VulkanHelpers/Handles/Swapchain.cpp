#include "VulkanHelpers/Handles/Swapchain.hpp"

Swapchain::Swapchain(VkSwapchainCreateInfoKHR const& createInfo, Device const& device) : _device(&device) {
    VkResult result = vkCreateSwapchainKHR(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a swapchain (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Swap chain created successully: <VkSwapchainKHR " << _handle << ">" << std::endl;
}

Swapchain::Swapchain(Swapchain&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

Swapchain::~Swapchain() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(_device->Handle(), _handle, VK_NULL_HANDLE);
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

        bool success = SDL_GetWindowSizeInPixels(window.Handle(), &width, &height);
        if (!success) {
            std::string error = "Could not get window size";
            throw std::runtime_error(error);
        }

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(
            actualExtent.width,
            surfaceCapabilities.surfaceCapabilities.minImageExtent.width,
            surfaceCapabilities.surfaceCapabilities.maxImageExtent.width
        );

        actualExtent.height = std::clamp(
            actualExtent.height,
            surfaceCapabilities.surfaceCapabilities.minImageExtent.height,
            surfaceCapabilities.surfaceCapabilities.maxImageExtent.height
        );

        return actualExtent;
    }
}

Swapchain& Swapchain::operator = (Swapchain&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
