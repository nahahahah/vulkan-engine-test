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
        Swapchain(VkSwapchainCreateInfoKHR const& createInfo, Device& device);
        ~Swapchain();

        VkSwapchainKHR Handle() { return _handle; }
        VkSwapchainKHR Handle() const { return _handle; }

        static VkExtent2D Extent2DFromSDLWindow(Window const& window, VkSurfaceCapabilities2KHR const& surfaceCapabilities);

        void CreateHandle(VkSwapchainCreateInfoKHR const& createInfo);
        void DestroyHandle();

    private:
        VkSwapchainKHR _handle = VK_NULL_HANDLE;
        Device& _device;
};

#endif // VK_WRAPPER_SWAPCHAIN_HPP
