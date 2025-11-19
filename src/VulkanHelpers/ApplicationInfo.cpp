#include "VulkanHelpers/ApplicationInfo.hpp"

ApplicationInfo::ApplicationInfo() {
    // structure type
    this->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    // versions
    this->apiVersion = VK_API_VERSION_1_4;
    this->applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    this->engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

    // names
    this->pApplicationName = "Vulkan application";
    this->pEngineName = "No engine";

    // extend application info
    this->pNext = VK_NULL_HANDLE;
}