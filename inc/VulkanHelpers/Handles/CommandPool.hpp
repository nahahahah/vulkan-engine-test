#ifndef VK_WRAPPER_COMMAND_POOL_HPP
#define VK_WRAPPER_COMMAND_POOL_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class CommandPool {
    public:
        CommandPool() = delete;
        CommandPool(VkCommandPoolCreateInfo const& createInfo, Device const& device, std::string const& label);
        CommandPool(CommandPool const& other) = delete;
        CommandPool(CommandPool&& other);
        ~CommandPool();

        CommandPool& operator = (CommandPool const& other) = delete;
        CommandPool& operator = (CommandPool&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }


        VkCommandPool Handle() { return _handle; }
        VkCommandPool Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkCommandPool _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_COMMAND_POOL_HPP
