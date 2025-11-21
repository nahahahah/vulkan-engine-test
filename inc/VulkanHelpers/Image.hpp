#ifndef VK_WRAPPER_IMAGE_HPP
#define VK_WRAPPER_IMAGE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Device.hpp"
#include "VulkanHelpers/Swapchain.hpp"

std::vector<VkImage> EnumerateSwapChainImages(Device const& device, Swapchain const& swapChain);

#endif // VK_WRAPPER_IMAGE_HPP
