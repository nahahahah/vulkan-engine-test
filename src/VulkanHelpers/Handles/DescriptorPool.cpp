#include "VulkanHelpers/Handles/DescriptorPool.hpp"

DescriptorPool::DescriptorPool(VkDescriptorPoolCreateInfo const& createInfo, Device& device) : _device(&device) {
    VkResult result = vkCreateDescriptorPool(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create a descriptor pool (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Descriptor pool created successfully: <VkDescriptorPool " << _handle << ">" << std::endl;
}

DescriptorPool::~DescriptorPool() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(_device->Handle(), _handle, VK_NULL_HANDLE);
        _handle = VK_NULL_HANDLE;
        std::clog << "Descriptor pool destroyed successfully" << std::endl;
    }
}
