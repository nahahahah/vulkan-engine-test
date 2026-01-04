#ifndef VK_WRAPPER_DEBUG_UTILS_MESSENGER_CREATE_INFO_HPP
#define VK_WRAPPER_DEBUG_UTILS_MESSENGER_CREATE_INFO_HPP

#include <string>
#include <functional>

#include <vulkan/vulkan.h>

VkDebugUtilsMessengerCreateInfoEXT GenerateDebugUtilsMessengerCreateInfo(PFN_vkDebugUtilsMessengerCallbackEXT DebugCallback);

#endif // VK_WRAPPER_DEBUG_UTILS_MESSENGER_CREATE_INFO_HPP
