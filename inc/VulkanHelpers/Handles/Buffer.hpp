#ifndef VK_WRAPPER_BUFFER_HPP
#define VK_WRAPPER_BUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

class Device;

class Buffer {
    public:
        Buffer() = delete;
        Buffer(VkBufferCreateInfo const& createInfo, Device const& device);
        Buffer(Buffer const& other) = delete;
        Buffer(Buffer&& other);
        ~Buffer();

        Buffer& operator = (Buffer const& other) = delete;
        Buffer& operator = (Buffer&& other);

        VkBuffer Handle() { return _handle; }
        VkBuffer Handle() const { return _handle; }

    private:
        VkBuffer _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_BUFFER_HPP
