#include "VulkanHelpers/Handles/Surface.hpp"

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"

Surface::Surface(Instance const& instance, Window const& window, std::string const& label)
    : _label(label), _instance(&instance) {
    if (!SDL_Vulkan_CreateSurface(window.Handle(), _instance->Handle(), VK_NULL_HANDLE, &_handle)) {
        std::string error = "Unable to create \"" + _label + "\" surface (status: " + std::string(SDL_GetError()) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" surface created successfully: <VkSurface " << _handle << ">" << std::endl;
}

Surface::Surface(Surface&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _instance = other._instance;
    other._instance = nullptr;
}

Surface::~Surface() {
    if (_handle != VK_NULL_HANDLE) {
        // destroy surface
        vkDestroySurfaceKHR(_instance->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" surface destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

Surface& Surface::operator = (Surface&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _instance = other._instance;
    other._instance = nullptr;

    return *this;
}
