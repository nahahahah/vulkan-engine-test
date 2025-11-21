#include "VulkanHelpers/PhysicalDeviceSurfaceInfo.hpp"

VkPhysicalDeviceSurfaceInfo2KHR GeneratePhysicalDeviceSurfaceInfo(Surface const& surface, void* next) {
    VkPhysicalDeviceSurfaceInfo2KHR info {};

    info.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    info.surface = surface.Handle();
    info.pNext = next;

    return info;
}
