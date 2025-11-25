#include "VulkanHelpers/Handles/PipelineLayout.hpp"

PipelineLayout::PipelineLayout(VkPipelineLayoutCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreatePipelineLayout(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a pipeline layout (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Pipeline layout created successfully: <VkPipelineLayout " << _handle << ">" << std::endl; 
}

PipelineLayout::~PipelineLayout() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Pipeline layout destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}