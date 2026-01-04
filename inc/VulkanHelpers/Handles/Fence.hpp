#ifndef VK_WRAPPER_FENCE_HPP
#define VK_WRAPPER_FENCE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Fence {
    public:
        Fence() = delete;
        Fence(VkFenceCreateInfo const& createInfo, Device const& device, std::string const& label);
        Fence(Fence const& other) = delete;
        Fence(Fence&& other);
        ~Fence();

        Fence& operator = (Fence const& other) = delete;
        Fence& operator = (Fence&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkFence Handle() { return _handle; }
        VkFence Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkFence _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_FENCE_HPP
