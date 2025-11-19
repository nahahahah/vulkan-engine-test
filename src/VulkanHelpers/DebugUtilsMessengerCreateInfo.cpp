#include "VulkanHelpers/DebugUtilsMessengerCreateInfo.hpp"

DebugUtilsMessengerCreateInfo::DebugUtilsMessengerCreateInfo(PFN_vkDebugUtilsMessengerCallbackEXT DebugCallback) {
    // structure type and flags    
    this->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    this->flags = 0;

    // message properties
    this->messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
        ;
    this->messageType = 
    //    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
        ;
    
    // callback and its optional parameter
    this->pfnUserCallback = DebugCallback;
    this->pUserData = VK_NULL_HANDLE;
    
    this->pNext = VK_NULL_HANDLE;
}