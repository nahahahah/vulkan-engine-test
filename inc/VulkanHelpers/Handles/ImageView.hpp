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
        ImageView(VkImageViewCreateInfo const& createInfo, Device const& device, std::string const& label);
        ImageView(ImageView const& other) = delete;
        ImageView(ImageView&& other);
        ~ImageView();

        ImageView& operator = (ImageView const& other) = delete;
        ImageView& operator = (ImageView&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkImageView Handle() { return _handle; }
        VkImageView Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkImageView _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_IMAGE_VIEW_HPP
