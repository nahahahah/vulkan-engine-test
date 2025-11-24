#ifndef VK_WRAPPER_SEMAPHORE_HPP
#define VK_WRAPPER_SEMAPHORE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Device.hpp"

class Semaphore {
    public:
        Semaphore() = default;
        Semaphore(VkSemaphoreCreateInfo const& createInfo, Device const& device);
        ~Semaphore();

        VkSemaphore Handle() { return _handle; }
        VkSemaphore Handle() const { return _handle; }

    private:
        VkSemaphore _handle = VK_NULL_HANDLE;
        Device const& _device;
};

#endif // VK_WRAPPER_SEMAPHORE_HPP
