#include "VulkanHelpers/Surface.hpp"

Surface::Surface(Instance const& instance, Window const& window) : _instance(instance) {
    if (!SDL_Vulkan_CreateSurface(window.Handle(), _instance.Handle(), VK_NULL_HANDLE, &_handle)) {
        std::string error = "Unable to create a surface (status: " + std::string(SDL_GetError()) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Surface created successfully: <VkSurface " << _handle << ">" << std::endl;
}

Surface::~Surface() {
    if (_handle != VK_NULL_HANDLE) {
        // destroy surface
        vkDestroySurfaceKHR(_instance.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Surface destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

bool Surface::IsSupportedByQueueFamily(PhysicalDevice const& physicalDevice, uint32_t queueFamilyIndex) {
    VkBool32 supportedSurface = VK_FALSE;
    VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice.Handle(), queueFamilyIndex, _handle, &supportedSurface);
    if (result != VK_SUCCESS) {
        std::string error = "Could not get surface support (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }

    return supportedSurface == VK_TRUE;
}
