#include "VulkanHelpers/Handles/Sampler.hpp"

Sampler::Sampler(VkSamplerCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkCreateSampler(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create \"" + _label + "\" sampler (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" sampler created successfully: <VkSampler " << _handle << ">" << std::endl;
}

Sampler::Sampler(Sampler&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
} 

Sampler::~Sampler() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroySampler(_device->Handle(), _handle, VK_NULL_HANDLE);
        _handle = VK_NULL_HANDLE;
    }
}

Sampler& Sampler::operator = (Sampler&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
