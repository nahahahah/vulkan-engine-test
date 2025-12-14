#include "VulkanHelpers/Handles/DescriptorSetLayout.hpp"

DescriptorSetLayout::DescriptorSetLayout(VkDescriptorSetLayoutCreateInfo const& createInfo, Device& device)
    : _device(&device) {
    VkResult result = vkCreateDescriptorSetLayout(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Couldn't create a descriptor set layout (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Descriptor set layout created successfully: <VkDescriptorSetLayout " << _handle << ">" << std::endl;
}

DescriptorSetLayout::~DescriptorSetLayout() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(_device->Handle(), _handle, VK_NULL_HANDLE);
        _handle = VK_NULL_HANDLE;
        std::clog << "Descriptor set layout destroyed successfully" << std::endl;
    }
}
