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
        Image(VkImage image, std::string const& label);
        Image(VkImageCreateInfo const& createInfo, Device const& device, std::string const& label);
        Image(Image const& other) = delete;
        Image(Image&& other);
        ~Image();

        Image& operator = (Image const& other) = delete;
        Image& operator = (Image&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }

        VkImage Handle() { return _handle; }
        VkImage Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkImage _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_IMAGE_HPP
