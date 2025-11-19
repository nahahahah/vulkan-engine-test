#ifndef VK_WRAPPER_QUEUE_FAMILY_PROPERTIES_HPP
#define VK_WRAPPER_QUEUE_FAMILY_PROPERTIES_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/PhysicalDevice.hpp"
#include "VulkanHelpers/QueueFlags.hpp"

struct QueueFamilyProperties : public VkQueueFamilyProperties2 {
    QueueFamilyProperties();

    static std::vector<QueueFamilyProperties> Enumerate(PhysicalDevice const& physicalDevice);
    static void Display(QueueFamilyProperties const& queueFamilyProperties);
};

#endif // VK_WRAPPER_QUEUE_FAMILY_PROPERTIES_HPP
