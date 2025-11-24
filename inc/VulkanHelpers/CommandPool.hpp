#ifndef VK_WRAPPER_COMMAND_POOL_HPP
#define VK_WRAPPER_COMMAND_POOL_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Device.hpp"

class CommandPool {
    public:
        CommandPool(VkCommandPoolCreateInfo const& createinfo, Device const& device);
        ~CommandPool();

        VkCommandPool Handle() { return _handle; }
        VkCommandPool Handle() const { return _handle; }

    private:
        VkCommandPool _handle = VK_NULL_HANDLE;
        Device const& _device;
};

#endif // VK_WRAPPER_COMMAND_POOL_HPP
