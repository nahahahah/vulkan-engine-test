#ifndef VK_WRAPPER_COLOR_SPACE_HPP
#define VK_WRAPPER_COLOR_SPACE_HPP

#include <iostream>

#include <vulkan/vulkan.h>

std::ostream& operator << (std::ostream& out, VkColorSpaceKHR const& colorSpace);

#endif // VK_WRAPPER_COLOR_SPACE_HPP
