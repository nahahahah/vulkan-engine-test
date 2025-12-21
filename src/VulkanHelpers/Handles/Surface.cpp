#include "VulkanHelpers/Handles/Surface.hpp"

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"

Surface::Surface(Instance const& instance, Window const& window) : _instance(&instance) {
    if (!SDL_Vulkan_CreateSurface(window.Handle(), _instance->Handle(), VK_NULL_HANDLE, &_handle)) {
        std::string error = "Unable to create a surface (status: " + std::string(SDL_GetError()) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Surface created successfully: <VkSurface " << _handle << ">" << std::endl;
}

Surface::Surface(Surface&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _instance = other._instance;
    other._instance = nullptr;
}

Surface::~Surface() {
    if (_handle != VK_NULL_HANDLE) {
        // destroy surface
        vkDestroySurfaceKHR(_instance->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Surface destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

Surface& Surface::operator = (Surface&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _instance = other._instance;
    other._instance = nullptr;

    return *this;
}
