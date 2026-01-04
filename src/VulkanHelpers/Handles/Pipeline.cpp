#include "VulkanHelpers/Handles/Pipeline.hpp"

Pipeline::Pipeline(VkGraphicsPipelineCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkCreateGraphicsPipelines(_device->Handle(), VK_NULL_HANDLE, 1, &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create \"" + _label + "\" graphics pipeline (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" graphics pipeline created successfully: <VkPipeline " << _handle << ">" << std::endl;
}

Pipeline::Pipeline(Pipeline&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;
}

Pipeline::~Pipeline() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyPipeline(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" graphics pipeline destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

Pipeline& Pipeline::operator = (Pipeline&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    return *this;
}
