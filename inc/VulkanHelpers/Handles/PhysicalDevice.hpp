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
        PhysicalDevice(PhysicalDevice const& other) = delete;
        PhysicalDevice(PhysicalDevice&& other);
        PhysicalDevice(VkPhysicalDevice physicalDevice);

        PhysicalDevice& operator = (PhysicalDevice const& other) = delete;
        PhysicalDevice& operator = (PhysicalDevice&& other);

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
};

class EnumeratedPhysicalDevices {
    public:
        using Iterator = std::vector<PhysicalDevice>::iterator;
        using ConstIterator = std::vector<PhysicalDevice>::const_iterator;
        using ConstReverseIterator = std::vector<PhysicalDevice>::const_reverse_iterator;

        EnumeratedPhysicalDevices() = default;
        EnumeratedPhysicalDevices(Instance const& instance);
        EnumeratedPhysicalDevices(EnumeratedPhysicalDevices const& other) = delete;
        EnumeratedPhysicalDevices(EnumeratedPhysicalDevices&& other) = default;
        /**
         * @note Nothing to destroy specifically. The physical devices will be freed internally by the Vulkan instance.
         */
        ~EnumeratedPhysicalDevices() = default;

        EnumeratedPhysicalDevices& operator = (EnumeratedPhysicalDevices const& other) = delete;
        EnumeratedPhysicalDevices& operator = (EnumeratedPhysicalDevices&& other) = default;

        PhysicalDevice& operator [] (size_t index);
        PhysicalDevice const& operator [] (size_t index) const;

        size_t size() { return _wrappers.size(); }
        
        VkPhysicalDevice const* const Handles() { return _handles.data(); }

        Iterator begin() { return _wrappers.begin(); }
        ConstIterator begin() const { return _wrappers.begin(); }
        
        Iterator end() { return _wrappers.end(); }
        ConstIterator end() const { return _wrappers.end(); }
        
        ConstIterator cbegin() const { return _wrappers.cbegin(); }
        ConstIterator cend() const { return _wrappers.cend(); }
        
        std::vector<PhysicalDevice>::reverse_iterator rbegin() { return _wrappers.rbegin(); }
        ConstReverseIterator rbegin() const { return _wrappers.rbegin(); }
        
        std::vector<PhysicalDevice>::reverse_iterator rend() { return _wrappers.rend(); }
        ConstReverseIterator rend() const{ return _wrappers.rend(); }
        
        ConstReverseIterator crbegin() const { return _wrappers.crbegin(); }
        ConstReverseIterator crend() const { return _wrappers.crend(); }

    private:
        std::vector<VkPhysicalDevice> _handles {};
        std::vector<PhysicalDevice> _wrappers {};
};

#endif // VK_WRAPPER_PHYSICAL_DEVICE_HPP
