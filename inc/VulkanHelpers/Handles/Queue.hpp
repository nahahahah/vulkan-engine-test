#ifndef VK_WRAPPER_QUEUE_HPP
#define VK_WRAPPER_QUEUE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Queue {
    public:
        Queue(VkDeviceQueueInfo2 const& queueInfo, Device const& device);
        ~Queue() = default;

        VkQueue Handle() { return _handle; }
        VkQueue Handle() const { return _handle; }

    private:
        VkQueue _handle = VK_NULL_HANDLE;
};

#endif // VK_WRAPPER_QUEUE_HPP
