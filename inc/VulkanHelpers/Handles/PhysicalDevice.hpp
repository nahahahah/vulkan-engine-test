#ifndef VK_WRAPPER_PHYSICAL_DEVICE_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Instance.hpp"
#include "VulkanHelpers/ParameterStructs/SurfaceFormat.hpp"

class Surface;

class PhysicalDevice {
    public:
        PhysicalDevice() = default;
        PhysicalDevice(VkPhysicalDevice physicalDevice);

        VkPhysicalDevice Handle() { return _handle; }
        VkPhysicalDevice Handle() const { return _handle; }

        bool IsSuitable(Surface const& surface) const;

        VkPhysicalDeviceProperties2 Properties() const;
        VkPhysicalDeviceFeatures2 Features() const;
        std::vector<VkQueueFamilyProperties2> QueueFamilyProperties() const;
        VkSurfaceCapabilities2KHR SurfaceCapabilities(VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) const;
        std::vector<VkSurfaceFormat2KHR> SurfaceFormats(VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) const;
        std::vector<VkPresentModeKHR> PresentModes(Surface const& surface) const;

        static std::vector<PhysicalDevice> Enumerate(Instance const& instance);

        bool IsSurfaceSupportedByQueueFamily(Surface const& surface, uint32_t queueFamilyIndex) const;

        VkPhysicalDeviceMemoryProperties2 MemoryProperties() const;

    private:
        VkPhysicalDevice _handle = nullptr;
        VkSurfaceCapabilities2KHR _surfaceCapabilities {};
        std::vector<VkSurfaceFormat2KHR> _surfaceFormats {};
        std::vector<VkPresentModeKHR> _presentModes {};
};

#endif // VK_WRAPPER_PHYSICAL_DEVICE_HPP
