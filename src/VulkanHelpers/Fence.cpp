#include "VulkanHelpers/Fence.hpp"

Fence::Fence(VkFenceCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateFence(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create fence (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Fence created successfully: <VkFence " << _handle << ">" << std::endl;
}

Fence::~Fence() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFence(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Fence destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    } 
}
