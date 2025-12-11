#ifndef VK_WRAPPER_BUFFER_HPP
#define VK_WRAPPER_BUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Buffer {
    public:
        Buffer(VkBufferCreateInfo const& createInfo, Device& device);
        ~Buffer();

        VkBuffer Handle() { return _handle; }
        VkBuffer Handle() const { return _handle; }

        VkMemoryRequirements2 MemoryRequirements(VkBufferMemoryRequirementsInfo2 info);

    private:
        VkBuffer _handle = VK_NULL_HANDLE;
        Device& _device;
};

#endif // VK_WRAPPER_BUFFER_HPP