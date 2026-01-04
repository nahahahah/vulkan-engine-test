#ifndef VK_WRAPPER_SWAPCHAIN_HPP
#define VK_WRAPPER_SWAPCHAIN_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <limits>

#include <vulkan/vulkan.h>


#include "SDLHelpers/Window.hpp"

class Device;

class Swapchain {
    public:
        Swapchain() = delete;
        Swapchain(VkSwapchainCreateInfoKHR const& createInfo, Device const& device, std::string const& label);
        Swapchain(Swapchain const& other) = delete;
        Swapchain(Swapchain&& other);
        ~Swapchain();

        Swapchain& operator = (Swapchain const& other) = delete;
        Swapchain& operator = (Swapchain&& other);
        
        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkSwapchainKHR Handle() { return _handle; }
        VkSwapchainKHR Handle() const { return _handle; }

        static VkExtent2D Extent2DFromSDLWindow(Window const& window, VkSurfaceCapabilities2KHR const& surfaceCapabilities);

    private:
        std::string _label = "";
        VkSwapchainKHR _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_SWAPCHAIN_HPP
