#ifndef VK_WRAPPER_QUEUE_HPP
#define VK_WRAPPER_QUEUE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"
#include "VulkanHelpers/Handles/Fence.hpp"

class Queue {
    public:
        Queue() = delete;
        Queue(VkDeviceQueueInfo2 const& queueInfo, Device const& device, std::string const& label);
        Queue(Queue const& other) = delete;
        Queue(Queue&& other);
        ~Queue() = default;

        Queue& operator = (Queue const& other) = delete;
        Queue& operator = (Queue&& other);

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }
        
        VkQueue Handle() { return _handle; }
        VkQueue Handle() const { return _handle; }

        void WaitIdle();

        void Submit(std::span<VkSubmitInfo2> submitInfos, Fence const* fence = nullptr);
        VkResult Present(VkPresentInfoKHR const& presentInfo);

    private:
        std::string _label = "";
        VkQueue _handle = VK_NULL_HANDLE;
};

#endif // VK_WRAPPER_QUEUE_HPP
