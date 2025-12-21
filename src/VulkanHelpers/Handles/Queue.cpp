#include "VulkanHelpers/Handles/Queue.hpp"

Queue::Queue(VkDeviceQueueInfo2 const& queueInfo, Device const& device) {
    vkGetDeviceQueue2(device.Handle(), &queueInfo, &_handle);
}

Queue::Queue(Queue&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;
}

Queue& Queue::operator = (Queue&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    return *this;
}
