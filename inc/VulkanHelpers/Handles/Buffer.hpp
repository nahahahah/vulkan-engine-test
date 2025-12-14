#ifndef VK_WRAPPER_BUFFER_HPP
#define VK_WRAPPER_BUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Buffer {
    public:
        Buffer(VkBufferCreateInfo const& createInfo, Device* device);
        Buffer(Buffer const& other) = delete;
        Buffer(Buffer&& other);
        ~Buffer();

        Buffer& operator = (Buffer&& other);

        VkBuffer Handle() { return _handle; }
        VkBuffer Handle() const { return _handle; }

        VkMemoryRequirements2 MemoryRequirements(VkBufferMemoryRequirementsInfo2 info);

    private:
        VkBuffer _handle = VK_NULL_HANDLE;
        Device* _device = nullptr;
};

#endif // VK_WRAPPER_BUFFER_HPP