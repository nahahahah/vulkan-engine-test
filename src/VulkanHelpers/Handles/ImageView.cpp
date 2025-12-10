#include "VulkanHelpers/Handles/ImageView.hpp"

ImageView::ImageView(VkImageViewCreateInfo const& createInfo, Device const& device) : _device(device) {
    try {
        CreateHandle(createInfo);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

ImageView::ImageView(ImageView&& other) : _device(other._device) {
    if (other._handle != VK_NULL_HANDLE) {
        _handle = other._handle;
        other._handle = VK_NULL_HANDLE;
    }
}

ImageView::~ImageView() {
    DestroyHandle();
}

void ImageView::CreateHandle(VkImageViewCreateInfo const& createInfo) {
     VkResult result = vkCreateImageView(_device.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a image view (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Image view created successfully: <VkImageView " << _handle << ">" << std::endl;
}

void ImageView::DestroyHandle() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyImageView(_device.Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "Image view destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}
