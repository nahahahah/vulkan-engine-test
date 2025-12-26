#ifndef VK_WRAPPER_IMAGE_HPP
#define VK_WRAPPER_IMAGE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Swapchain.hpp"

class Device;

class Image {
    public:
        Image() = delete;
        Image(VkImage image);
        Image(VkImageCreateInfo const& createInfo, Device const& device);
        Image(Image const& other) = delete;
        Image(Image&& other);
        ~Image();

        Image& operator = (Image const& other) = delete;
        Image& operator = (Image&& other);

        VkImage Handle() { return _handle; }
        VkImage Handle() const { return _handle; }

    private:
        VkImage _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_IMAGE_HPP
