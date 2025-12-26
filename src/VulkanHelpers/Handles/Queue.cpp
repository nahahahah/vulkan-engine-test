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

void Queue::WaitIdle() {
    VkResult result = vkQueueWaitIdle(_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to wait for idleing of the queue (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Queue is now idle" << std::endl;
}

void Queue::Submit(std::span<VkSubmitInfo2> submitInfos, Fence const* fence) {
    VkResult result = vkQueueSubmit2(
        _handle,
        static_cast<uint32_t>(submitInfos.size()),
        submitInfos.data(),
        ((fence != nullptr) ? (fence->Handle()) : (VK_NULL_HANDLE))
    );
    if (result != VK_SUCCESS) {
        std::string error = "Unable to submit commands with the queue (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Commands submited successfully" << std::endl;
}

VkResult Queue::Present(VkPresentInfoKHR const& presentInfo) {
    return vkQueuePresentKHR(_handle, &presentInfo);
}