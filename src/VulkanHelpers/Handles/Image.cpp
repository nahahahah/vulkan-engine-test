#include "VulkanHelpers/Handles/Image.hpp"

#include "VulkanHelpers/Handles/Device.hpp"

Image::Image(VkImage image, std::string const& label) : _label(label), _handle(image) {}

Image::Image(VkImageCreateInfo const& createInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkCreateImage(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not create \"" + _label + "\" image (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" image created successfully: <VkImage " << _handle << ">" << std::endl;
}

Image::Image(Image&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
} 

Image::~Image() {
    if (_device != nullptr
     && _device->Handle() != VK_NULL_HANDLE
     && _handle != VK_NULL_HANDLE) {
        vkDestroyImage(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" image destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

Image& Image::operator = (Image&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
