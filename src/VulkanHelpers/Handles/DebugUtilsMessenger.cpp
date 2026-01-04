#include "VulkanHelpers/Handles/DebugUtilsMessenger.hpp"

DebugUtilsMessenger::DebugUtilsMessenger(VkDebugUtilsMessengerCreateInfoEXT const& createInfo, Instance const& instance, std::string const& label)
    : _label(label), _instance(&instance) {
    auto CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(_instance->Handle(), "vkCreateDebugUtilsMessengerEXT"));
    if (CreateDebugUtilsMessengerEXT == VK_NULL_HANDLE) {
        std::string error = "Unable to get instance process address for vkCreateDebugUtilsMessengerEXT";
        throw std::runtime_error(error);
    }

    VkResult result = CreateDebugUtilsMessengerEXT(_instance->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create \"" + _label + "\" debug utils messenger (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" debug utils messenger created successfully: <VkDebugUtilsMessengerEXT " << _handle << ">" << std::endl;
}

DebugUtilsMessenger::DebugUtilsMessenger(DebugUtilsMessenger&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _instance = other._instance;
    other._instance = nullptr;
}

DebugUtilsMessenger::~DebugUtilsMessenger() {
    if (_handle != VK_NULL_HANDLE) {
        // destroy debug messenger
        auto DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(_instance->Handle(), "vkDestroyDebugUtilsMessengerEXT"));
        if (DestroyDebugUtilsMessengerEXT == VK_NULL_HANDLE) {
            std::cerr << "Unable to get instance process address for vkDestroyDebugUtilsMessengerEXT" << std::endl;
        }

        else {
            DestroyDebugUtilsMessengerEXT(_instance->Handle(), _handle, VK_NULL_HANDLE);
            std::clog << "\"" << _label << "\" debug messenger destroyed successfully" << std::endl;
            _handle = VK_NULL_HANDLE;
        }
    }
}

DebugUtilsMessenger& DebugUtilsMessenger::operator = (DebugUtilsMessenger&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _instance = other._instance;
    other._instance = nullptr;

    return *this;
}
