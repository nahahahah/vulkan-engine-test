#ifndef VK_WRAPPER_SWAPCHAIN_HPP
#define VK_WRAPPER_SWAPCHAIN_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <limits>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

#include "SDLHelpers/Window.hpp"

class Swapchain {
    public:
        Swapchain() = delete;
        Swapchain(VkSwapchainCreateInfoKHR const& createInfo, Device const& device);
        Swapchain(Swapchain const& other) = delete;
        Swapchain(Swapchain&& other);
        ~Swapchain();

        Swapchain& operator = (Swapchain const& other) = delete;
        Swapchain& operator = (Swapchain&& other);
        
        VkSwapchainKHR Handle() { return _handle; }
        VkSwapchainKHR Handle() const { return _handle; }

        static VkExtent2D Extent2DFromSDLWindow(Window const& window, VkSurfaceCapabilities2KHR const& surfaceCapabilities);

    private:
        VkSwapchainKHR _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_SWAPCHAIN_HPP
