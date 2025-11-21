#include "VulkanHelpers/PhysicalDeviceSynchronization2Features.hpp"

VkPhysicalDeviceSynchronization2Features GeneratePhysicalDeviceSynchronization2Features() {
    VkPhysicalDeviceSynchronization2Features features {};

    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
    features.synchronization2 = VK_TRUE;
    features.pNext = VK_NULL_HANDLE;

    return features;    
}
