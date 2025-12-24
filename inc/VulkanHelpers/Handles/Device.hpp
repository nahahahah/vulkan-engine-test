#ifndef VK_WRAPPER_DEVICE_HPP
#define VK_WRAPPER_DEVICE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/PhysicalDevice.hpp"
#include "VulkanHelpers/Handles/Buffer.hpp"

class Swapchain;

class Device {
    public:
        Device() = delete;
        Device(VkDeviceCreateInfo const& createInfo, PhysicalDevice const& physicalDevice);
        Device(Device const& other) = delete;
        Device(Device&& other);
        ~Device();

        Device& operator = (Device const& other) = delete;
        Device& operator = (Device&& other);

        VkDevice Handle() { return _handle; }
        VkDevice Handle() const { return _handle; }

        void WaitIdle();

        // fences related methods
        void WaitForFences(std::span<VkFence> const& fences, VkBool32 waitAll = VK_TRUE, uint64_t timeout = UINT64_MAX);
        void ResetFences(std::span<VkFence> const& fences);

        // memory related methods
        void MapMemory(VkMemoryMapInfo const& memoryMapInfo, void** data);
        void UnmapMemory(VkMemoryUnmapInfo const& memoryUnmapInfo);

        // buffer related methods
        VkMemoryRequirements2 BufferMemoryRequirements(VkBufferMemoryRequirementsInfo2 const& info);
        void BindBufferMemory(std::span<VkBindBufferMemoryInfo> bindInfos);

        // image related methods
        std::vector<VkImage> SwapchainImages(Device const& device, Swapchain const& swapchain);
        VkMemoryRequirements2 ImageMemoryRequirements(VkImageMemoryRequirementsInfo2 const& info);
        void BindImageMemory(std::span<VkBindImageMemoryInfo> bindInfos);

    private:
        VkDevice _handle = VK_NULL_HANDLE;
};

#endif // VK_WRAPPER_DEVICE_HPP
