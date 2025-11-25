#include "VulkanHelpers/Handles/ShaderModule.hpp"

ShaderModule::ShaderModule(VkShaderModuleCreateInfo const& createInfo, Device const& device) : _device(device) {
    VkResult result = vkCreateShaderModule(device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a shader module: (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::cerr << "Shader module created successfully: <VkShaderModule " << _handle << ">" << std::endl;
}

ShaderModule::~ShaderModule() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Shader module destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}
