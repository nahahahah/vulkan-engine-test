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
        Surface() = delete;
        Surface(Instance const& instance, Window const& window, std::string const& label);
        Surface(Surface const& other) = delete;
        Surface(Surface&& other);
        ~Surface();

        Surface& operator = (Surface const& other) = delete;
        Surface& operator = (Surface&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkSurfaceKHR Handle() { return _handle; }
        VkSurfaceKHR Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkSurfaceKHR _handle = VK_NULL_HANDLE;
        Instance const* _instance = nullptr;
};

#endif // VK_WRAPPER_SURFACE_HPP
