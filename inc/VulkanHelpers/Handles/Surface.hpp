#ifndef VK_WRAPPER_SURFACE_HPP
#define VK_WRAPPER_SURFACE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Instance.hpp"
#include "SDLHelpers/Window.hpp"

class PhysicalDevice;

class Surface {
    public:
        Surface(Instance const& instance, Window const& window);
        ~Surface();

        VkSurfaceKHR Handle() { return _handle; }
        VkSurfaceKHR Handle() const { return _handle; }

    private:
        VkSurfaceKHR _handle = VK_NULL_HANDLE;
        Instance const& _instance;
};

#endif // VK_WRAPPER_SURFACE_HPP
