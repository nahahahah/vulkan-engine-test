#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <vector>
#include <utility>
#include <chrono>
#include <numbers>
#include <cstdint>
#include <set>

#include <vulkan/vulkan.h>

#include "SDLHelpers/Window.hpp"

#include "VulkanHelpers/CreateInfos.hpp"
#include "VulkanHelpers/Flags.hpp"
#include "VulkanHelpers/Handles.hpp"
#include "VulkanHelpers/ParameterEnums.hpp"
#include "VulkanHelpers/ParameterInfos.hpp"
#include "VulkanHelpers/ParameterStructs.hpp"

#include "Assets.hpp"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily {};
    std::optional<uint32_t> presentFamily {};
    std::optional<uint32_t> transferFamily {};

    bool IsComplete() { graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapchainSupportDetails {
    VkSurfaceCapabilities2KHR capabilities {};
    std::vector<VkSurfaceFormat2KHR> formats {};
    std::vector<VkPresentModeKHR> presentModes {};
};

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

class Application {
    public: // constructors / destructor
        Application();
        ~Application();

        void Run();

    private: // methods
        void InitSDL();
        void QuitSDL();

        void InitWindow();
        void InitVulkan();
        void SetupDebugMessenger();
        void CreateInstance();
        void CreateSurface();
        void SelectPhysicalDevice();
        void CreateDevice();
        void CreateQueues();
        void CreateSwapchain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateDescriptorSetLayout();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateVertexBuffer();
        void CreateIndexBuffer();
        void CreateUniformBuffer();
        void CreateDescriptorPool();
        void CreateDescriptorSets();
        void CreateCommandBuffers();
        void CreateSynchronizationObjects();

        void MainLoop();
        
        QueueFamilyIndices FindQueueFamilies(PhysicalDevice const& physicalDevice);
        SwapchainSupportDetails QuerySwapchainSupport(PhysicalDevice const& physicalDevice);
        bool IsPhysicalDeviceSuitable(PhysicalDevice const& physicalDevice);
        void CleanUpSwapchain();
        void RecreateSwapchain();


    private: // attributes
        std::unique_ptr<Window> _window = nullptr;
        std::unique_ptr<DebugUtilsMessenger> _debugUtilsMessenger = nullptr;
        std::unique_ptr<Instance> _instance = nullptr;
        std::unique_ptr<Surface> _surface = nullptr;
        std::unique_ptr<EnumeratedPhysicalDevices> _physicalDevices = nullptr;
        std::unique_ptr<PhysicalDevice> _physicalDevice = nullptr;
        std::unique_ptr<Device> _device = nullptr;
        std::unique_ptr<Queue> _graphicsQueue = nullptr;
        std::unique_ptr<Queue> _presentQueue = nullptr;
        std::unique_ptr<Swapchain> _swapchain = nullptr;
        std::vector<ImageView> _imageViews {};
        std::unique_ptr<RenderPass> _renderPass = nullptr;
        std::unique_ptr<DescriptorSetLayout> _descriptorSetLayout = nullptr;
        std::unique_ptr<PipelineLayout> _pipelineLayout = nullptr;
        std::unique_ptr<Pipeline> _graphicsPipeline = nullptr;
        std::vector<Framebuffer> _framebuffers {};
        std::unique_ptr<CommandPool> _commandPool = nullptr;
        std::unique_ptr<Buffer> _vertexBuffer = nullptr;
        std::unique_ptr<Buffer> _indexBuffer = nullptr;
        std::unique_ptr<Buffer> _uniformBuffer = nullptr;
        std::unique_ptr<DescriptorPool> _descriptorPool = nullptr;
        
        std::vector<CommandBuffer> _commandBuffers {};
};

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
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties,
    uint32_t queueFamilyIndex,
    Queue& graphicsQueue
);

std::pair<Buffer, DeviceMemory> CreateIndexBuffer(
    std::span<uint16_t> indices,
    PhysicalDevice const& physicalDevice,
    Device& device,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties,
    uint32_t queueFamilyIndex,
    Queue& graphicsQueue
);

void CopyBuffer(
    uint32_t queueFamilyIndex,
    Device& device,
    Queue& graphicsQueue,
    Buffer& src,
    Buffer& dst,
    VkDeviceSize size
);

void UpdateUniformBuffer(void* mappedUniformBuffer, float ratio);

#endif // APPLICATION_HPP