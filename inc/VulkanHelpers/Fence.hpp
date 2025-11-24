#ifndef VK_WRAPPER_FENCE_HPP
#define VK_WRAPPER_FENCE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Device.hpp"

class Fence {
    public:
        Fence() = default;
        Fence(VkFenceCreateInfo const& createInfo, Device const& device);
        ~Fence();

        VkFence Handle() { return _handle; }
        VkFence Handle() const { return _handle; }

        VkFence const* HandleAddress() const { return &_handle; }

    private:
        VkFence _handle = VK_NULL_HANDLE;
        Device const& _device;
};

#endif // VK_WRAPPER_FENCE_HPP
