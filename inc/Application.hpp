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

    bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapchainSupportDetails {
    VkSurfaceCapabilities2KHR capabilities {};
    std::vector<VkSurfaceFormat2KHR> formats {};
    std::vector<VkPresentModeKHR> presentModes {};
};

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

inline std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

inline std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

inline std::vector<Vertex> shapeVertices = {
    Vertex { Math::Vector2(-0.5f, -0.5f), Math::Vector3(1.0f, 0.0f, 0.0f) },
    Vertex { Math::Vector2( 0.5f, -0.5f), Math::Vector3(0.0f, 1.0f, 0.0f) },
    Vertex { Math::Vector2( 0.5f,  0.5f), Math::Vector3(0.0f, 0.0f, 1.0f) },
    Vertex { Math::Vector2(-0.5f,  0.5f), Math::Vector3(1.0f, 1.0f, 1.0f) }
};

inline std::vector<uint16_t> shapeIndices = {
    0, 1, 2, 2, 3, 0
};


class Application {
    public: // constructors / destructor
        Application();
        ~Application();

        void Run();

    private: // methods
#ifndef NDEBUG
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
            void* pUserData
        );
#endif

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
        void GetSwapchainImages();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateDescriptorSetLayout();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateVertexBuffer();
        void CreateIndexBuffer();
        void CreateUniformBuffers();
        void CreateDescriptorPool();
        void CreateDescriptorSets();
        void CreateCommandBuffers();
        void CreateSynchronizationObjects();

        void MainLoop();
        
        void RecordCommandBuffer(CommandBuffer& commandBuffer, uint32_t imageIndex);
        void UpdateUniformBuffer(uint32_t currentImage);

        QueueFamilyIndices FindQueueFamilies(PhysicalDevice const& physicalDevice);
        SwapchainSupportDetails QuerySwapchainSupport(PhysicalDevice const& physicalDevice);
        bool IsPhysicalDeviceSuitable(PhysicalDevice const& physicalDevice);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        
        VkSurfaceFormat2KHR ChooseSwapSurfaceFormat(std::span<VkSurfaceFormat2KHR> availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(std::span<VkPresentModeKHR> availablePresentModes);
        VkExtent2D ChooseSwapExtent(VkSurfaceCapabilities2KHR const& surfaceCapabilities);

        void CreateBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkSharingMode sharingMode,
            VkMemoryPropertyFlags properties,
            std::unique_ptr<Buffer>& buffer,
            std::unique_ptr<DeviceMemory>& bufferMemory
        );

        void CopyBuffer(Buffer& src, Buffer& dst, VkDeviceSize size);

        void CleanupSwapchain();
        void RecreateSwapchain();


    private: // attributes
        std::unique_ptr<Window> _window = nullptr;
        std::unique_ptr<Instance> _instance = nullptr;
        std::unique_ptr<Surface> _surface = nullptr;
        std::unique_ptr<DebugUtilsMessenger> _debugUtilsMessenger = nullptr;
        std::unique_ptr<PhysicalDeviceCollection> _physicalDevices = nullptr;
        std::unique_ptr<PhysicalDevice> _physicalDevice = nullptr;
        std::unique_ptr<Device> _device = nullptr;
        std::unique_ptr<Queue> _graphicsQueue = nullptr;
        std::unique_ptr<Queue> _presentQueue = nullptr;
        std::unique_ptr<Swapchain> _swapchain = nullptr;
        VkFormat _swapchainImageFormat = VkFormat::VK_FORMAT_UNDEFINED;
        VkExtent2D _swapchainExtent {};
        std::vector<VkImage> _swapchainImages {};
        std::vector<ImageView> _swapchainImageViews {};
        std::unique_ptr<RenderPass> _renderPass = nullptr;
        std::unique_ptr<DescriptorSetLayout> _descriptorSetLayout = nullptr;
        std::unique_ptr<PipelineLayout> _pipelineLayout = nullptr;
        std::unique_ptr<Pipeline> _graphicsPipeline = nullptr;
        std::vector<Framebuffer> _framebuffers {};
        std::unique_ptr<CommandPool> _commandPool = nullptr;

        std::unique_ptr<Buffer> _vertexBuffer = nullptr;
        std::unique_ptr<DeviceMemory> _vertexBufferMemory = nullptr;
        std::unique_ptr<Buffer> _indexBuffer = nullptr;
        std::unique_ptr<DeviceMemory> _indexBufferMemory = nullptr;

        std::vector<std::unique_ptr<Buffer>> _uniformBuffers {};
        std::vector<std::unique_ptr<DeviceMemory>> _uniformBuffersMemory {};
        std::vector<void*> _uniformBuffersMapped {};

        std::unique_ptr<DescriptorPool> _descriptorPool = nullptr;
        std::unique_ptr<DescriptorSetCollection> _descriptorSets = nullptr;

        std::unique_ptr<CommandBufferCollection> _commandBuffers = nullptr;

        std::vector<Semaphore> _imageAvailableSemaphores {};
        std::vector<Semaphore> _renderFinishedSemaphores {};
        std::vector<Fence> _inFlightFences {};
        
        uint32_t _frameIndex = 0;
        bool _framebufferResized = false;
        SDL_Event _event {};
        bool _running = false;
};

#endif // APPLICATION_HPP
