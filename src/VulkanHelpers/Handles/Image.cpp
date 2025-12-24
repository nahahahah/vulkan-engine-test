#include "VulkanHelpers/Handles/Image.hpp"

#include "VulkanHelpers/Handles/Device.hpp"

Image::Image(VkImageCreateInfo const& createInfo, Device const& device) : _device(&device) {
    VkResult result = vkCreateImage(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create a image (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Image created successfully: <VkImage " << _handle << ">" << std::endl;
}

Image::Image(Image&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
} 

Image::~Image() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyImage(_device->Handle(), _handle, VK_NULL_HANDLE);
        _handle = VK_NULL_HANDLE;
    }
}

Image& Image::operator = (Image&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
