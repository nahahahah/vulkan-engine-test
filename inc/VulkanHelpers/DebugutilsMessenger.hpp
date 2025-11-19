#ifndef VK_WRAPPER_DEBUG_UTILS_MESSENGER_HPP
#define VK_WRAPPER_DEBUG_UTILS_MESSENGER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Instance.hpp"

class DebugUtilsMessenger {
    public:
        DebugUtilsMessenger(Instance const& instance, VkDebugUtilsMessengerCreateInfoEXT const& createInfo);
        ~DebugUtilsMessenger();

        VkDebugUtilsMessengerEXT Handle() { return _handle; }
        VkDebugUtilsMessengerEXT const Handle() const { return _handle; }

    private:
        VkDebugUtilsMessengerEXT _handle = VK_NULL_HANDLE;
        Instance const& _instance;
};

#endif // VK_WRAPPER_DEBUG_UTILS_MESSENGER_HPP