#include "VulkanHelpers/Handles/ImageView.hpp"

ImageView::ImageView(VkImageViewCreateInfo const& createInfo, Device const& device) : _device(&device) {
    VkResult result = vkCreateImageView(_device->Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a image view (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Image view created successfully: <VkImageView " << _handle << ">" << std::endl;
}

ImageView::ImageView(ImageView&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

ImageView::~ImageView() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyImageView(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Image view destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

ImageView& ImageView::operator = (ImageView&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
