#include "VulkanHelpers/Handles/Pipeline.hpp"

Pipeline::Pipeline(VkGraphicsPipelineCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateGraphicsPipelines(device.Handle(), VK_NULL_HANDLE, 1, &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create a graphics pipeline (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Graphics pipeline created successfully: <VkPipeline " << _handle << ">" << std::endl;
}

Pipeline::~Pipeline() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyPipeline(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Graphics pipeline destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}