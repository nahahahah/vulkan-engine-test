#ifndef VK_WRAPPER_COMMAND_POOL_HPP
#define VK_WRAPPER_COMMAND_POOL_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class CommandPool {
    public:
        CommandPool(VkCommandPoolCreateInfo const& createInfo, Device& device);
        ~CommandPool();

        VkCommandPool Handle() { return _handle; }
        VkCommandPool Handle() const { return _handle; }

    private:
        VkCommandPool _handle = VK_NULL_HANDLE;
        Device* _device = nullptr;
};

#endif // VK_WRAPPER_COMMAND_POOL_HPP
