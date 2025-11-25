#include "VulkanHelpers/Handles/DebugutilsMessenger.hpp"

DebugUtilsMessenger::DebugUtilsMessenger(Instance const& instance, VkDebugUtilsMessengerCreateInfoEXT const& createInfo) : _instance(instance) {
    auto CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(_instance.Handle(), "vkCreateDebugUtilsMessengerEXT"));
    if (CreateDebugUtilsMessengerEXT == VK_NULL_HANDLE) {
        std::string error = "Unable to get instance process address for vkCreateDebugUtilsMessengerEXT";
        throw std::runtime_error(error);
    }

    VkResult result = CreateDebugUtilsMessengerEXT(instance.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a debug utils messenger (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
}

DebugUtilsMessenger::~DebugUtilsMessenger() {
    if (_handle != VK_NULL_HANDLE) {
        // destroy debug messenger
        auto DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(_instance.Handle(), "vkDestroyDebugUtilsMessengerEXT"));
        if (DestroyDebugUtilsMessengerEXT == VK_NULL_HANDLE) {
            std::cerr << "Unable to get instance process address for vkCreateDebugUtilsMessengerEXT" << std::endl;
        }

        else {
            DestroyDebugUtilsMessengerEXT(_instance.Handle(), _handle, VK_NULL_HANDLE);
            std::clog << "Debug messenger destroyed successfully" << std::endl;
            _handle = VK_NULL_HANDLE;
        }
    }
}
