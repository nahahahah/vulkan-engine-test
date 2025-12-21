#ifndef VK_WRAPPER_QUEUE_HPP
#define VK_WRAPPER_QUEUE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"

class Queue {
    public:
        Queue() = delete;
        Queue(VkDeviceQueueInfo2 const& queueInfo, Device const& device);
        Queue(Queue const& other) = delete;
        Queue(Queue&& other);
        ~Queue() = default;

        Queue& operator = (Queue const& other) = delete;
        Queue& operator = (Queue&& other);

        VkQueue Handle() { return _handle; }
        VkQueue Handle() const { return _handle; }

    private:
        VkQueue _handle = VK_NULL_HANDLE;
};

#endif // VK_WRAPPER_QUEUE_HPP
