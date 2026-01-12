#ifndef VK_WRAPPER_PHYSICAL_DEVICE_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Instance.hpp"
#include "VulkanHelpers/ParameterStructs/SurfaceFormat.hpp"
#include "VulkanHelpers/ParameterStructs/FormatProperties.hpp"

class Surface;

class PhysicalDevice {
    public:
        PhysicalDevice() = delete;
        PhysicalDevice(PhysicalDevice const& other) = delete;
        PhysicalDevice(PhysicalDevice&& other);
        PhysicalDevice(VkPhysicalDevice physicalDevice);
        ~PhysicalDevice() = default;

        PhysicalDevice& operator = (PhysicalDevice const& other) = delete;
        PhysicalDevice& operator = (PhysicalDevice&& other);

        VkPhysicalDevice Handle() { return _handle; }
        VkPhysicalDevice Handle() const { return _handle; }

        VkPhysicalDeviceProperties2 Properties() const;
        VkPhysicalDeviceFeatures2 Features() const;
        std::vector<VkQueueFamilyProperties2> QueueFamilyProperties() const;
        VkSurfaceCapabilities2KHR SurfaceCapabilities(VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) const;
        std::vector<VkSurfaceFormat2KHR> SurfaceFormats(VkPhysicalDeviceSurfaceInfo2KHR const& surfaceInfo) const;
        std::vector<VkPresentModeKHR> PresentModes(Surface const& surface) const;
        VkFormatProperties2 FormatProperties(VkFormat format);

        bool IsSurfaceSupportedByQueueFamily(Surface const& surface, uint32_t queueFamilyIndex) const;

        VkPhysicalDeviceMemoryProperties2 MemoryProperties() const;

    private:
        VkPhysicalDevice _handle = nullptr;
};

class PhysicalDeviceCollection {
    public:
        using Iterator = std::vector<PhysicalDevice>::iterator;
        using ConstIterator = std::vector<PhysicalDevice>::const_iterator;
        using ConstReverseIterator = std::vector<PhysicalDevice>::const_reverse_iterator;

        PhysicalDeviceCollection() = default;
        PhysicalDeviceCollection(Instance const& instance, std::string const& label);
        PhysicalDeviceCollection(PhysicalDeviceCollection const& other) = delete;
        PhysicalDeviceCollection(PhysicalDeviceCollection&& other) = default;
        /**
         * @note Nothing to destroy specifically. The physical devices will be freed internally by the Vulkan instance.
         */
        ~PhysicalDeviceCollection() = default;

        PhysicalDeviceCollection& operator = (PhysicalDeviceCollection const& other) = delete;
        PhysicalDeviceCollection& operator = (PhysicalDeviceCollection&& other) = default;

        PhysicalDevice& operator [] (size_t index);
        PhysicalDevice const& operator [] (size_t index) const;

        size_t size() { return _wrappers.size(); }
        
        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }

        VkPhysicalDevice const* Handles() { return _handles.data(); }

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
        std::string _label = "";
        std::vector<VkPhysicalDevice> _handles {};
        std::vector<PhysicalDevice> _wrappers {};
};

#endif // VK_WRAPPER_PHYSICAL_DEVICE_HPP
