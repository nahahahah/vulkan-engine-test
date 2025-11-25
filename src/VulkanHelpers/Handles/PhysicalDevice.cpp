#include "VulkanHelpers/Handles/PhysicalDevice.hpp"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice const& physicalDevice) : _handle(physicalDevice) {}

std::vector<PhysicalDevice> PhysicalDevice::Enumerate(Instance const& instance) {
    // get physical devices count
    uint32_t count = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance.Handle(), &count, VK_NULL_HANDLE);
    if (result != VkResult::VK_SUCCESS) {
        std::string error = "Unable to enumerate physical devices (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Physical devices enumerated successfully (1st call, count: " << count << ")" << std::endl;

    // should not happen on a modern PC because the CPU can be used
    if (count == 0) {
        std::string error = "No physical devices found";
        throw std::runtime_error(error);
    }

    // enumerate physical devices
    std::vector<VkPhysicalDevice> tmpPhysicalDevices(count, VK_NULL_HANDLE);
    result = vkEnumeratePhysicalDevices(instance.Handle(), &count, tmpPhysicalDevices.data());
    if (result != VkResult::VK_SUCCESS) {
        std::string error = "Unable to enumerate physical devices (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Physical devices enumerated successfully (2nd call, retrieved in array)" << std::endl;

    std::vector<PhysicalDevice> physicalDevices;
    physicalDevices.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        physicalDevices.emplace_back(tmpPhysicalDevices[i]);
    }

    return physicalDevices;
}
