#ifndef VK_WRAPPER_SHADER_MODULE_HPP
#define VK_WRAPPER_SHADER_MODULE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class ShaderModule {
    public:
        ShaderModule() = delete;
        ShaderModule(VkShaderModuleCreateInfo const& createInfo, Device const& device, std::string const& label);
        ShaderModule(ShaderModule const& other) = delete;
        ShaderModule(ShaderModule&& other);
        ~ShaderModule();

        ShaderModule& operator = (ShaderModule const& other) = delete;
        ShaderModule& operator = (ShaderModule&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }

        VkShaderModule Handle() { return _handle; }
        VkShaderModule Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkShaderModule _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_SHADER_MODULE_HPP
