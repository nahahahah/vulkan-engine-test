#ifndef VK_WRAPPER_DEBUG_UTILS_MESSENGER_HPP
#define VK_WRAPPER_DEBUG_UTILS_MESSENGER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Instance.hpp"

class DebugUtilsMessenger {
    public:
        DebugUtilsMessenger() = delete;
        DebugUtilsMessenger(VkDebugUtilsMessengerCreateInfoEXT const& createInfo, Instance const& instance);
        DebugUtilsMessenger(DebugUtilsMessenger const& other) = delete;
        DebugUtilsMessenger(DebugUtilsMessenger&& other);
        ~DebugUtilsMessenger();

        DebugUtilsMessenger& operator = (DebugUtilsMessenger const& other) = delete;
        DebugUtilsMessenger& operator = (DebugUtilsMessenger&& other);

        VkDebugUtilsMessengerEXT Handle() { return _handle; }
        VkDebugUtilsMessengerEXT Handle() const { return _handle; }

    private:
        VkDebugUtilsMessengerEXT _handle = VK_NULL_HANDLE;
        Instance const* _instance = nullptr;
};

#endif // VK_WRAPPER_DEBUG_UTILS_MESSENGER_HPP
