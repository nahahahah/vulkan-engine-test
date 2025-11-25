#include "VulkanHelpers/Handles/CommandPool.hpp"

CommandPool::CommandPool(VkCommandPoolCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateCommandPool(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create command pool (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Command pool created successfully: <VkCommandPool " << _handle << ">" << std::endl;
}

CommandPool::~CommandPool() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Command pool destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }

}
