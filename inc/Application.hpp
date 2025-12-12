#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <utility>

#include <vulkan/vulkan.h>

#include "SDLHelpers/Window.hpp"

#include "VulkanHelpers/CreateInfos.hpp"
#include "VulkanHelpers/Flags.hpp"
#include "VulkanHelpers/Handles.hpp"
#include "VulkanHelpers/ParameterEnums.hpp"
#include "VulkanHelpers/ParameterInfos.hpp"
#include "VulkanHelpers/ParameterStructs.hpp"

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

#include "Assets.hpp"

// debug callback
#ifndef NDEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* pUserData
);
#endif

/// TODO: VERY BAD IDEA HERE. Find a way to NOT give access to the handles (especially giving the right to create/destroy it)
void HandleFrameInvalidity(
    PhysicalDevice const& physicalDevice,
    Device& device,
    Surface const& surface,
    VkSwapchainCreateInfoKHR const& swapchainCreateInfo,
    Swapchain& swapchain,
    VkFormat imageFormat,
    std::vector<VkImage>& swapchainImages,
    std::vector<ImageView>& swapchainImageViews,
    std::vector<Framebuffer>& framebuffers,
    RenderPass const& renderPass
);

uint32_t FindMemoryType(PhysicalDevice const& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

std::pair<Buffer, DeviceMemory> CreateBuffer(
    PhysicalDevice const& physicalDevice,
    Device& device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties
);

std::pair<Buffer, DeviceMemory> CreateVertexBuffer(
    std::span<Vertex> vertices,
    PhysicalDevice const& physicalDevice,
    Device& device,
    [[maybe_unused]] VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties
);

#endif // APPLICATION_HPP