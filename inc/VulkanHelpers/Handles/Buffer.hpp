#ifndef VK_WRAPPER_BUFFER_HPP
#define VK_WRAPPER_BUFFER_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

class Device;

class Buffer {
    public:
        Buffer() = delete;
        Buffer(VkBufferCreateInfo const& createInfo, Device const& device, std::string const& label);
        Buffer(Buffer const& other) = delete;
        Buffer(Buffer&& other);
        ~Buffer();

        Buffer& operator = (Buffer const& other) = delete;
        Buffer& operator = (Buffer&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkBuffer Handle() { return _handle; }
        VkBuffer Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkBuffer _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_BUFFER_HPP
