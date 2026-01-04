#ifndef VK_WRAPPER_SAMPLER_HPP
#define VK_WRAPPER_SAMPLER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Sampler {
    public:
        Sampler() = delete;
        Sampler(VkSamplerCreateInfo const& createInfo, Device const& device, std::string const& label);
        Sampler(Sampler const& other) = delete;
        Sampler(Sampler&& other);
        ~Sampler();

        Sampler& operator = (Sampler const& other) = delete;
        Sampler& operator = (Sampler&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkSampler Handle() { return _handle; }
        VkSampler Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkSampler _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_SAMPLER_HPP
