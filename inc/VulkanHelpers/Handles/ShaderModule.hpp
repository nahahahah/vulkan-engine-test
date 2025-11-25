#ifndef VK_WRAPPER_SHADER_MODULE_HPP
#define VK_WRAPPER_SHADER_MODULE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class ShaderModule {
    public:
        ShaderModule(VkShaderModuleCreateInfo const& createInfo, Device const& device);
        ~ShaderModule();

        VkShaderModule Handle() { return _handle; }
        VkShaderModule Handle() const { return _handle; }

    private:
        VkShaderModule _handle = VK_NULL_HANDLE;
        Device const& _device;
};

#endif // VK_WRAPPER_SHADER_MODULE_HPP
