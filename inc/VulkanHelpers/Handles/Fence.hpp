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
        Fence() = default;
        Fence(Fence const& other) = delete;
        Fence(Fence&& other);
        Fence(VkFenceCreateInfo const& createInfo, Device const& device);
        ~Fence();

        VkFence Handle() { return _handle; }
        VkFence Handle() const { return _handle; }

    private:
        VkFence _handle = VK_NULL_HANDLE;
        Device const& _device;
};

void WaitForFences(Device const& device, std::span<VkFence> const& fences, VkBool32 waitAll = VK_TRUE, uint64_t timeout = UINT64_MAX);
void ResetFences(Device const& device, std::span<VkFence> const& fences);

#endif // VK_WRAPPER_FENCE_HPP
