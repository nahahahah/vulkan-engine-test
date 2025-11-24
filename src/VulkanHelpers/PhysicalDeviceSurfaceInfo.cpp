#include "VulkanHelpers/PhysicalDeviceSurfaceInfo.hpp"

VkPhysicalDeviceSurfaceInfo2KHR GeneratePhysicalDeviceSurfaceInfo(Surface const& surface, void* next) {
    VkPhysicalDeviceSurfaceInfo2KHR info {};

    // structure type
    info.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    
    // handle
    info.surface = surface.Handle();
    
    // extend surface info
    info.pNext = next;

    return info;
}
