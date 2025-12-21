#ifndef VK_WRAPPER_IMAGE_VIEW_HPP
#define VK_WRAPPER_IMAGE_VIEW_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class ImageView {
    public:
        ImageView() = delete;
        ImageView(VkImageViewCreateInfo const& createInfo, Device const& device);
        ImageView(ImageView const& other) = delete;
        ImageView(ImageView&& other);
        ~ImageView();

        ImageView& operator = (ImageView const& other) = delete;
        ImageView& operator = (ImageView&& other);

        VkImageView Handle() { return _handle; }
        VkImageView Handle() const { return _handle; }

    private:
        VkImageView _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_IMAGE_VIEW_HPP
