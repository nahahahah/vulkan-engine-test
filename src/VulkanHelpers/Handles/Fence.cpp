#include "VulkanHelpers/Handles/Fence.hpp"

Fence::Fence(VkFenceCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateFence(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create fence (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Fence created successfully: <VkFence " << _handle << ">" << std::endl;
}

Fence::Fence(Fence&& other) : _device(other._device) {
    if (other._handle != VK_NULL_HANDLE) {
        _handle = other._handle;
        other._handle = VK_NULL_HANDLE;
    }
}

Fence::~Fence() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFence(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Fence destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    } 
}

void WaitForFences(Device const& device, std::span<VkFence> const& fences, VkBool32 waitAll, uint64_t timeout) {
    VkResult result = vkWaitForFences(device.Handle(), static_cast<uint32_t>(fences.size()), fences.data(), waitAll, timeout);
    if (result != VK_SUCCESS) {
        std::string error = "Could not wait for fences (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Waited for fences successfully" << std::endl;
}

void ResetFences(Device const& device, std::span<VkFence> const& fences) {
    VkResult result = vkResetFences(device.Handle(), static_cast<uint32_t>(fences.size()), fences.data());
    if (result != VK_SUCCESS) {
        std::string error = "Could not reset fences (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Reset fences successfully" << std::endl;
}
