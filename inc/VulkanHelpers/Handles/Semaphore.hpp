#ifndef VK_WRAPPER_SEMAPHORE_HPP
#define VK_WRAPPER_SEMAPHORE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Semaphore {
    public:
        Semaphore() = delete;
        Semaphore(Semaphore const& other) = delete;
        Semaphore(Semaphore&& other);
        Semaphore(VkSemaphoreCreateInfo const& createInfo, Device const& device, std::string const& label);
        ~Semaphore();

        Semaphore& operator = (Semaphore const& other) = delete;
        Semaphore& operator = (Semaphore&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkSemaphore Handle() { return _handle; }
        VkSemaphore Handle() const { return _handle; }

    private:
        std::string _label = "";
        VkSemaphore _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
};

#endif // VK_WRAPPER_SEMAPHORE_HPP
