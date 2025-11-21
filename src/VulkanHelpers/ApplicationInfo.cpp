#include "VulkanHelpers/ApplicationInfo.hpp"

VkApplicationInfo GenerateApplicationInfo() {
    VkApplicationInfo applicationInfo {};

    // structure type
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    // versions
    applicationInfo.apiVersion = VK_API_VERSION_1_4;
    applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

    // names
    applicationInfo.pApplicationName = "Vulkan application";
    applicationInfo.pEngineName = "gammaShade";

    // extend application info
    applicationInfo.pNext = VK_NULL_HANDLE;

    return applicationInfo;
}
