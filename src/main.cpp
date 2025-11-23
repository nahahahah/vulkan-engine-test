#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>
#include <iomanip>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <fstream>
#include <filesystem>

#define SDL_MAIN_HANDLED
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include "SDLHelpers/Window.hpp"

#include "VulkanHelpers/ApplicationInfo.hpp"
#include "VulkanHelpers/LayerProperties.hpp"
#include "VulkanHelpers/ExtensionProperties.hpp"
#include "VulkanHelpers/InstanceCreateInfo.hpp"
#include "VulkanHelpers/Version.hpp"
#include "VulkanHelpers/SampleCountFlags.hpp"
#include "VulkanHelpers/QueueFlags.hpp"
#include "VulkanHelpers/SurfaceTransformFlags.hpp"
#include "VulkanHelpers/ImageUsageFlags.hpp"
#include "VulkanHelpers/ColorSpace.hpp"
#include "VulkanHelpers/PresentMode.hpp"
#include "VulkanHelpers/PhysicalDeviceLimits.hpp"
#include "VulkanHelpers/PhysicalDeviceSparseProperties.hpp"
#include "VulkanHelpers/QueueFamilyProperties.hpp"
#include "VulkanHelpers/ExtensionSupportCheck.hpp"
#include "VulkanHelpers/PhysicalDeviceProperties.hpp"
#include "VulkanHelpers/PhysicalDeviceFeatures.hpp"
#include "VulkanHelpers/SurfaceFormat.hpp"
#include "VulkanHelpers/DebugUtilsMessengerCreateInfo.hpp"
#include "VulkanHelpers/Instance.hpp"
#include "VulkanHelpers/DebugutilsMessenger.hpp"
#include "VulkanHelpers/Surface.hpp"
#include "VulkanHelpers/PhysicalDevice.hpp"
#include "VulkanHelpers/SurfaceCapabilities.hpp"
#include "VulkanHelpers/PhysicalDeviceSurfaceInfo.hpp"
#include "VulkanHelpers/Swapchain.hpp"
#include "VulkanHelpers/DeviceQueueCreateInfo.hpp"
#include "VulkanHelpers/PhysicalDeviceSynchronization2Features.hpp"
#include "VulkanHelpers/DeviceCreateInfo.hpp"
#include "VulkanHelpers/Device.hpp"
#include "VulkanHelpers/DeviceQueueInfo.hpp"
#include "VulkanHelpers/Queue.hpp"
#include "VulkanHelpers/SwapchainCreateInfo.hpp"
#include "VulkanHelpers/Image.hpp"
#include "VulkanHelpers/ImageViewCreateInfo.hpp"
#include "VulkanHelpers/ImageView.hpp"
#include "VulkanHelpers/ShaderModuleCreateInfo.hpp"
#include "VulkanHelpers/ShaderModule.hpp"
#include "VulkanHelpers/PipelineShaderStageCreateInfo.hpp"
#include "VulkanHelpers/PipelineDynamicStateCreateInfo.hpp"
#include "VulkanHelpers/PipelineVertexInputStateCreateInfo.hpp"
#include "VulkanHelpers/PipelineInputAssemblyStateCreateInfo.hpp"
#include "VulkanHelpers/Viewport.hpp"
#include "VulkanHelpers/Rect2D.hpp"
#include "VulkanHelpers/PipelineViewportStateCreateInfo.hpp"
#include "VulkanHelpers/PipelineRasterizationStateCreateInfo.hpp"
#include "VulkanHelpers/PipelineMultisampleStateCreateInfo.hpp"
#include "VulkanHelpers/PipelineDepthStencilStateCreateInfo.hpp"
#include "VulkanHelpers/PipelineColorBlendAttachmentState.hpp"
#include "VulkanHelpers/PipelineColorBlendStateCreateInfo.hpp"
#include "VulkanHelpers/PipelineLayoutCreateInfo.hpp"
#include "VulkanHelpers/PipelineLayout.hpp"
#include "VulkanHelpers/AttachmentDescription.hpp"
#include "VulkanHelpers/AttachmentReference.hpp"
#include "VulkanHelpers/SubpassDescription.hpp"
#include "VulkanHelpers/SubpassDependency.hpp"
#include "VulkanHelpers/RenderPassCreateInfo.hpp"
#include "VulkanHelpers/RenderPass.hpp"
#include "VulkanHelpers/GraphicsPipelineCreateInfo.hpp"
#include "VulkanHelpers/Pipeline.hpp"
#include "VulkanHelpers/FramebufferCreateInfo.hpp"
#include "VulkanHelpers/Framebuffer.hpp"

// debug callback
#ifndef NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* pUserData
) {
    (void)(pUserData);

    std::clog << "[ ";
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            std::clog << "VERBOSE";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            std::clog << "INFO";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            std::clog << "WARNING";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            std::clog << "ERROR";
            break;
        }

        default: {
            break;
        }
    }

    std::clog << "::";
    
    switch (messageType) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
            std::clog << "GENERAL"; 
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
            std::clog << "VALIDATION";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
            std::clog << "PERFORMANCE";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: {
            std::clog << "DEVICE ADRESS BINDING";
            break; 
        }

        default: {
            break;
        }
    }

    std::cout << " ] ";

    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}
#endif

int main(int argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

    std::clog << "Vulkan header version: " << VK_HEADER_VERSION << std::endl;

    Window window;
    SDL_Event event {};
    bool running = false;
    std::unique_ptr<Instance> instance = nullptr;
#ifndef NDEBUG
    std::unique_ptr<DebugUtilsMessenger> debugUtilsMessenger = nullptr;
    constexpr bool enableValidationLayers = true;
#else
    constexpr bool enableValidationLayers = false;
#endif
    std::unique_ptr<Surface> surface = nullptr;
    VkSurfaceCapabilities2KHR choosenPhysicalDeviceSurfaceCapabilities {};
    std::unique_ptr<PhysicalDevice> physicalDevice = nullptr;
    std::optional<uint32_t> queueFamilyIndexWithGraphicsCapabilities = std::nullopt;
    std::optional<uint32_t> queueFamilyIndexWithPresentCapabilities = std::nullopt;
    std::unique_ptr<Device> device = nullptr;
    std::unique_ptr<Queue> graphicsQueue = nullptr;
    std::unique_ptr<Queue> presentQueue = nullptr;
    std::unique_ptr<Swapchain> swapChain = nullptr;
    std::vector<std::unique_ptr<ImageView>> swapChainImageViews {};
    std::unique_ptr<ShaderModule> vertexShaderModule = nullptr;
    std::unique_ptr<ShaderModule> fragmentShaderModule = nullptr;
    std::unique_ptr<PipelineLayout> pipelineLayout = nullptr;
    std::unique_ptr<RenderPass> renderPass = nullptr;
    std::unique_ptr<Pipeline> graphicsPipeline = nullptr;
    std::vector<std::unique_ptr<Framebuffer>> frameBuffers;
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers {};
    std::vector<VkSemaphore> imageAvailableSemaphores {};
    std::vector<VkSemaphore> renderFinishedSemaphores {};
    std::vector<VkFence> inFlightFences {};
    bool framebufferResized = false;

    constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    auto CleanOnExit = [&](int code) {
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            if (imageAvailableSemaphores[i] != VK_NULL_HANDLE) {
                vkDestroySemaphore(device->Handle(), imageAvailableSemaphores[i], VK_NULL_HANDLE);
                std::clog << "'Image available' semaphore #" << i << " destroyed successfully" << std::endl;
                imageAvailableSemaphores[i] = VK_NULL_HANDLE;
            }

            if (renderFinishedSemaphores[i] != VK_NULL_HANDLE) {
                vkDestroySemaphore(device->Handle(), renderFinishedSemaphores[i], VK_NULL_HANDLE);
                std::clog << "'Render finished' semaphore #" << i << " destroyed successfully" << std::endl;
                renderFinishedSemaphores[i] = VK_NULL_HANDLE;
            }

            if (inFlightFences[i] != VK_NULL_HANDLE) {
                vkDestroyFence(device->Handle(), inFlightFences[i], VK_NULL_HANDLE);
                std::clog << "'In flight' fence #" << i << " destroyed successfully" << std::endl;
                inFlightFences[i] = VK_NULL_HANDLE;
            }   
        }

        if (commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device->Handle(), commandPool, VK_NULL_HANDLE);
            std::clog << "Command pool destroyed successfully" << std::endl;
            commandPool = VK_NULL_HANDLE;
        }

        SDL_Quit();

        return code;
    };
    
    // init SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Unable to initialize the SDL (reason: " << SDL_GetError() << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "SDL initialized successfully" << std::endl;

    try {
        auto applicationInfo = GenerateApplicationInfo(); // create application info
        std::vector<char const*> validationLayers = { "VK_LAYER_KHRONOS_validation" }; // validation layers to use
        auto instanceLayersProperties = EnumerateLayerProperties(); // enumerate instance layers properties
        std::clog << "Instance layers properties:" << std::endl;
        std::clog << instanceLayersProperties << std::endl; // display instance layers properties 

        bool validationLayersSupported = AreValidationLayerSupported(validationLayers, instanceLayersProperties); // check validation layers support and enable them if available
        if (enableValidationLayers && !validationLayersSupported) {
            std::cerr << "Validation layers requested, but not available" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }

        // get enabled instance extensions
        Uint32 enabledExtensionsCount = 0;
        char const* const* enabledExtensionsArray = SDL_Vulkan_GetInstanceExtensions(&enabledExtensionsCount);
        std::vector<char const*> enabledExtensions {};
        for (int i = 0; i < static_cast<int>(enabledExtensionsCount); ++i) {
            enabledExtensions.emplace_back(enabledExtensionsArray[i]);
        }

        enabledExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // for MacOS, to avoid VK_ERROR_INCOMPATIBLE_DRIVER on vkCreateInstance
        enabledExtensions.emplace_back("VK_KHR_get_surface_capabilities2");
        if (enableValidationLayers) {
            enabledExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        std::clog << "Enabled extensions count retrieved successfully from SDL_Vulkan_GetInstanceExtensions (count: " << enabledExtensions.size() << ")" << std::endl;
        std::clog << "Enabled extensions:" << std::endl;
        for (const char* enabledExtension : enabledExtensions) {
            std::clog << " - " << enabledExtension << std::endl;
        }

        auto instanceExtensionsProperties = EnumerateInstanceExtensionProperties();

        std::clog << "Instance extensions:" << std::endl;
        std::clog << instanceExtensionsProperties << std::endl;

        DisplaySupportedExtensionsFromSDLVulkan(enabledExtensions, instanceExtensionsProperties);

    #ifndef NDEBUG
        // a pointer to nullptr by default since validation layers may not be enabled so debug utils will not be enabled as well (by default)
        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo {};

        if (enableValidationLayers) {
            debugUtilsMessengerCreateInfo = GenerateDebugUtilsMessengerCreateInfo(DebugCallback);
        }

        auto instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR, &debugUtilsMessengerCreateInfo);
    #else
        auto instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
    #endif

        instance = std::make_unique<Instance>(instanceCreateInfo); // create instance

    #ifndef NDEBUG
        debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(*instance, debugUtilsMessengerCreateInfo); // create the global debug messenger
    #endif

        surface = std::make_unique<Surface>(*instance, window); // create the surface from SDL

        std::vector<PhysicalDevice> physicalDevices = PhysicalDevice::Enumerate(*instance); // enumerate physical devices

        // specify device extensions
        std::vector<char const*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
        };

        // save preferred physical device properties
        VkSurfaceFormat2KHR preferredFormat {};
        VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        VkExtent2D swapchainExtent {};
        uint32_t imageCount = 0;
        VkSurfaceTransformFlagBitsKHR currentTransform {};

        // enumerate each physical device's properties
        std::ios_base::fmtflags formatFlags = std::clog.flags();
        for (PhysicalDevice const& pd : physicalDevices) {
            std::clog << "Physical device found: <VkPhysicalDevice " << pd.Handle() << ">" << std::endl;
            
            auto physicalDeviceProperties = GeneratePhysicalDeviceProperties(pd); // get physical device's properties
            auto physicalDeviceFeatures = GeneratePhysicalDeviceFeatures(pd); // get physical device's features
            auto physicalDeviceQueueFamilyProperties = EnumerateQueueFamilyProperties(pd); // get physical device's queue family properties
        
            int queueFamilyIndex = 0;
            std::clog << " - Queue families (count: " << physicalDeviceQueueFamilyProperties.size() << ")" << std::endl;
            for (VkQueueFamilyProperties2 const& queueFamilyProperties : physicalDeviceQueueFamilyProperties) {
                if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    queueFamilyIndexWithGraphicsCapabilities = queueFamilyIndex; 
                }

                // check that the retrieved surface is supported by a specific queue family
                VkBool32 supportedSurface = surface->IsSupportedByQueueFamily(pd, queueFamilyIndex);
                if (supportedSurface) {
                    queueFamilyIndexWithPresentCapabilities = queueFamilyIndex;
                }
            }

            // enumerate device extensions
            auto deviceExtensionsProperties = EnumerateDeviceExtensionProperties(pd); // enumerate device extensions
            bool deviceExtensionsSupported = AreDeviceExtensionsSupported(deviceExtensions, deviceExtensionsProperties);
            
            std::clog << (deviceExtensionsSupported ? "All device extensions are supported" : "Some or all device extensions aren't supported") << std::endl;

            auto physicalDeviceSurfaceInfo = GeneratePhysicalDeviceSurfaceInfo(*surface); // get surface info
            auto surfaceCapabilities = GenerateSurfaceCapabilities(pd, physicalDeviceSurfaceInfo); // get capabilities of the surface
            auto surfaceFormats = EnumerateSurfaceFormats(pd, physicalDeviceSurfaceInfo); // get formats of the surface
            auto presentModes = EnumeratePresentModes(pd, *surface); // get present modes of the surface

            // check is swapchain has mandatory properties
            bool swapChainIsAdequate = false;
            if (deviceExtensionsSupported) {
                swapChainIsAdequate = !surfaceFormats.empty() && !presentModes.empty();
            }

            // get adequate surface format
            preferredFormat = surfaceFormats[0];
            for (VkSurfaceFormat2KHR const& surfaceFormat : surfaceFormats) {
                if (
                    surfaceFormat.surfaceFormat.format     == VK_FORMAT_B8G8R8A8_SRGB           &&
                    surfaceFormat.surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
                ) {
                    preferredFormat = surfaceFormat;
                }
            }

            // get adequate surface mode
            preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR;
            for (VkPresentModeKHR const& presentMode : presentModes) {
                if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    preferredPresentMode = presentMode;
                }
            }

            swapchainExtent = Swapchain::Extent2DFromSDLWindow(window, surfaceCapabilities); // get future swapchain extent
            imageCount = surfaceCapabilities.surfaceCapabilities.minImageCount + 1;

            if (surfaceCapabilities.surfaceCapabilities.maxImageCount > 0
            && imageCount > surfaceCapabilities.surfaceCapabilities.maxImageCount) {
                imageCount = surfaceCapabilities.surfaceCapabilities.maxImageCount;
            }

            currentTransform = surfaceCapabilities.surfaceCapabilities.currentTransform;

            // check if device is suitable
            if (std::string deviceName = std::string(physicalDeviceProperties.properties.deviceName);
                physicalDeviceProperties.properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && deviceName.find("Microsoft Direct3D") == std::string::npos
            && physicalDeviceFeatures.features.geometryShader
            && queueFamilyIndexWithGraphicsCapabilities.has_value()
            && queueFamilyIndexWithPresentCapabilities.has_value()
            && deviceExtensionsSupported
            && swapChainIsAdequate
            ) {
                std::clog << "Current physical device selected" << std::endl;
                choosenPhysicalDeviceSurfaceCapabilities = surfaceCapabilities;
                physicalDevice = std::make_unique<PhysicalDevice>(pd);
            }
        }
        std::clog.flags(formatFlags);

        // check if a suitable device was found
        if (physicalDevice == VK_NULL_HANDLE || !queueFamilyIndexWithGraphicsCapabilities.has_value() || !queueFamilyIndexWithPresentCapabilities.has_value()) {
            std::cerr << "Unable to get a suitable physical device" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }

        // specify queues create info
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {}; 
        std::set<uint32_t> uniqueQueueFamilies = {
            queueFamilyIndexWithGraphicsCapabilities.value(),
            queueFamilyIndexWithPresentCapabilities.value()
        };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            queueCreateInfos.push_back(GenerateDeviceQueueCreateInfo(queueFamily, &queuePriority));
        }

        VkPhysicalDeviceFeatures2 physicalDeviceFeatures {}; // specify device enabled features
        auto physicalDeviceSynchronizationFeature2 = GeneratePhysicalDeviceSynchronization2Features(); // enable synchronization2 feature

        auto deviceCreateInfo = GenerateDeviceCreateInfo(queueCreateInfos, deviceExtensions, validationLayers, physicalDeviceFeatures, 0, &physicalDeviceSynchronizationFeature2);
        device = std::make_unique<Device>(deviceCreateInfo, physicalDevice->Handle()); // create device
    
        auto deviceGraphicsQueueInfo = GenerateDeviceQueueInfo(0, queueFamilyIndexWithGraphicsCapabilities.value());
        graphicsQueue = std::make_unique<Queue>(deviceGraphicsQueueInfo, *device); // get device graphics queue

        VkDeviceQueueInfo2 devicePresentQueueInfo = GenerateDeviceQueueInfo(0, queueFamilyIndexWithPresentCapabilities.value());
        presentQueue = std::make_unique<Queue>(devicePresentQueueInfo, *device); // get device present queue

        auto swapChainCreateInfo = GenerateSwapchainCreateInfo(
            preferredFormat.surfaceFormat.colorSpace,
            swapchainExtent,
            preferredFormat.surfaceFormat.format,
            queueFamilyIndexWithGraphicsCapabilities.value(),
            queueFamilyIndexWithPresentCapabilities.value(),
            imageCount,
            preferredPresentMode,
            currentTransform,
            *surface
        );
        swapChain = std::make_unique<Swapchain>(swapChainCreateInfo, *device); // create swapchain

        std::vector<VkImage> swapChainImages = EnumerateSwapChainImages(*device, *swapChain); // retrieve swap chain images

        swapChainImageViews.resize(swapChainImages.size()); // resize swap chain images views
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT); // resize command buffers
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT); // resize image available semaphores
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT); // resize render finished semaphores
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT); // resize fences vectors 

        for (size_t i = 0; i < swapChainImages.size(); ++i) {
            auto swapChainImageViewCreateInfo = GenerateImageViewCreateInfo(preferredFormat.surfaceFormat.format, swapChainImages[i]);
            swapChainImageViews[i] = std::make_unique<ImageView>(swapChainImageViewCreateInfo, *device); // create swap chain image view
        }
        
        std::vector<char> vertexShaderFileBuffer;
        auto vertexShaderModuleCreateInfo = GenerateShaderModuleCreateInfo("resources/shaders/triangle.vertex.spv", vertexShaderFileBuffer);
        vertexShaderModule = std::make_unique<ShaderModule>(vertexShaderModuleCreateInfo, *device); // create vertex shader module

        std::vector<char> fragmentShaderFileBuffer;
        auto fragmentShaderModuleCreateInfo = GenerateShaderModuleCreateInfo("resources/shaders/triangle.fragment.spv", fragmentShaderFileBuffer);
        fragmentShaderModule = std::make_unique<ShaderModule>(fragmentShaderModuleCreateInfo, *device); // create fragment shader module
        
        std::string shaderMainFunctionName = "main";
        auto vertexShaderPipelineStageCreateInfo = GeneratePipelineShaderStageCreateInfo(*vertexShaderModule, shaderMainFunctionName, VK_SHADER_STAGE_VERTEX_BIT); // create vertex shader pipeline stage
        auto fragmentShaderPipelineStageCreateInfo = GeneratePipelineShaderStageCreateInfo(*fragmentShaderModule, shaderMainFunctionName, VK_SHADER_STAGE_FRAGMENT_BIT); // create fragment shader pipeline stage

        std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos = { vertexShaderPipelineStageCreateInfo, fragmentShaderPipelineStageCreateInfo };
        std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        auto pipelineDynamicStateCreateInfo = GeneratePipelineDynamicStateCreateInfo(dynamicStates); // specify pipeline dynamic states

        auto pipelineVertexInputStateCreateInfo = GeneratePipelineVertexInputStateCreateInfo(); // specify pipeline vertex inputs
        auto pipelineInputAssemblyStateCreateInfo = GeneratePipelineInputAssemblyStateCreateInfo(); // specify pipeline input assemblies
        
        std::vector<VkViewport> viewport = { 
            GenerateViewport(static_cast<float>(swapchainExtent.width), static_cast<float>(swapchainExtent.height)) // specify viewport
        };
        std::vector<VkRect2D> scissor = { GenerateRect2D(swapchainExtent) }; // specify scissor
        auto pipelineViewportStateCreateInfo = GeneratePipelineViewportStateCreateInfo(viewport, scissor);

        auto pipelineRasterizationStateCreateInfo = GeneratePipelineRasterizationStateCreateInfo(); // specify pipeline rasterization state
        auto pipelineMultisampleStateCreateInfo = GeneratePipelineMultisampleStateCreateInfo(); // specify pipeline multisampling state
        //auto pipelineDepthStencilStateCreateInfo = GeneratePipelineDepthStencilStateCreateInfo(); // specify pipeline depth stencil state

        std::vector<VkPipelineColorBlendAttachmentState> pipelineColorBlendAttachmentState =  { GeneratePipelineColorBlendAttachmentState() }; // specify pipeline color blend attachment state
        auto pipelineColorBlendStateCreateInfo = GeneratePipelineColorBlendStateCreateInfo(pipelineColorBlendAttachmentState);

        auto pipelineLayoutCreateInfo = GeneratePipelineLayoutCreateInfo(); // create pipeline layout
        pipelineLayout = std::make_unique<PipelineLayout>(pipelineLayoutCreateInfo, *device);

        std::vector<VkAttachmentDescription2> attachmentDescription = { GenerateAttachmentDescription(preferredFormat.surfaceFormat.format) };  
        std::vector<VkAttachmentReference2> attachmentReference = { GenerateAttachmentReference() }; // create attachment reference
        std::vector<VkSubpassDescription2> subpassDescription = { GenerateSubpassDescription(attachmentReference) };
        std::vector<VkSubpassDependency2> subpassDependency = { GenerateSubpassDependency() }; // specify subpass dependencies
        auto renderPassCreateInfo = GenerateRenderPassCreateInfo(attachmentDescription, subpassDependency, subpassDescription);
        renderPass = std::make_unique<RenderPass>(renderPassCreateInfo, *device);

        // create graphics pipeline
        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = GenerateGraphicsPipelineCreateInfo(
            pipelineDynamicStateCreateInfo,
            pipelineVertexInputStateCreateInfo,
            pipelineInputAssemblyStateCreateInfo,
            pipelineViewportStateCreateInfo,
            pipelineRasterizationStateCreateInfo,
            pipelineMultisampleStateCreateInfo,
            pipelineColorBlendStateCreateInfo,
            *pipelineLayout,
            pipelineShaderStageCreateInfos,
            *renderPass
        );
        graphicsPipeline = std::make_unique<Pipeline>(graphicsPipelineCreateInfo, *device);

        // create frame buffers
        frameBuffers.resize(swapChainImageViews.size());
        for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
            std::vector<VkImageView> attachments = { swapChainImageViews[i]->Handle() }; // create frame buffer
            auto frameBufferCreateInfo = GenerateFramebufferCreateInfo(swapchainExtent, attachments, *renderPass);
            frameBuffers[i] = std::make_unique<Framebuffer>(frameBufferCreateInfo, *device);
        }

        // create command pool
        VkCommandPoolCreateInfo commandPoolCreateInfo {};
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.pNext = VK_NULL_HANDLE;
        commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndexWithGraphicsCapabilities.value();
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

        VkResult createCommandPoolResult = vkCreateCommandPool(device->Handle(), &commandPoolCreateInfo, VK_NULL_HANDLE, &commandPool);
        if (createCommandPoolResult != VK_SUCCESS) {
            std::cerr << "Could not create command pool (status: " << createCommandPoolResult << ")" << std::endl;
            CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Command pool created successfully: <VkCommandPool " << commandPool << ">" << std::endl;

        // create command buffer
        VkCommandBufferAllocateInfo commandBufferAllocateInfo {};
        commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        commandBufferAllocateInfo.commandPool = commandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.pNext = VK_NULL_HANDLE;
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

        VkResult allocateCommandBufferResult = vkAllocateCommandBuffers(device->Handle(), &commandBufferAllocateInfo, commandBuffers.data());
        if (allocateCommandBufferResult != VK_SUCCESS) {
            std::cerr << "Unable to allocate a command buffer (status: " << allocateCommandBufferResult << ")" << std::endl;
            CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Command buffers allocated successfully" << std::endl;

        // create synchronization primitive objects
        VkSemaphoreCreateInfo semaphoreCreateInfo {};
        semaphoreCreateInfo.flags = 0;
        semaphoreCreateInfo.pNext = VK_NULL_HANDLE;
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceCreateInfo {};
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // to avoid deadlock on the first frame
        fenceCreateInfo.pNext = VK_NULL_HANDLE;
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            VkResult createImageAvailableSemaphoreResult = vkCreateSemaphore(device->Handle(), &semaphoreCreateInfo, VK_NULL_HANDLE, &imageAvailableSemaphores[i]);
            if (createImageAvailableSemaphoreResult != VK_SUCCESS) {
                std::cerr << "Unable to create 'Image available' semaphore (status: " << createImageAvailableSemaphoreResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            std::clog << "'Image available' semaphore created successfully: <VkSemaphore " << imageAvailableSemaphores[i] << ">" << std::endl;
            
            VkResult createRenderFinishedSemaphoreResult = vkCreateSemaphore(device->Handle(), &semaphoreCreateInfo, VK_NULL_HANDLE, &renderFinishedSemaphores[i]);
            if (createRenderFinishedSemaphoreResult != VK_SUCCESS) {
                std::cerr << "Unable to create 'Render finished' semaphore (status: " << createRenderFinishedSemaphoreResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            std::clog << "'Render finished' semaphore created successfully: <VkSemaphore " << renderFinishedSemaphores[i] << ">" << std::endl;

            VkResult createInFlightFenceResult = vkCreateFence(device->Handle(), &fenceCreateInfo, VK_NULL_HANDLE, &inFlightFences[i]);
            if (createInFlightFenceResult != VK_SUCCESS) {
                std::cerr << "Unable to create 'In flight' fence (status: " << createInFlightFenceResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            std::clog << "'Render finished' fence created successfully: <VkFence " << inFlightFences[i] << ">" << std::endl;
        }

        running = true;
        uint32_t frameIndex = 0;
        while (running) {
            uint32_t imageIndex = 0;
            VkSemaphore& acquireSemaphore = imageAvailableSemaphores[frameIndex];
            VkSemaphore& submitSemaphore = renderFinishedSemaphores[frameIndex];
            VkFence& frameFence = inFlightFences[frameIndex];
            VkCommandBuffer& commandBuffer = commandBuffers[frameIndex];

            VkResult waitForFencesResult = vkWaitForFences(device->Handle(), 1, &frameFence, VK_TRUE, UINT64_MAX);
            if (waitForFencesResult != VK_SUCCESS) {
                std::cerr << "Could not wait for fences (status: " << waitForFencesResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            //std::clog << "Waited for fences successfully" << std::endl;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                }

                if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                    framebufferResized = true;
                    std::clog << "Window resized to " << event.window.data1 << "x" << event.window.data2 << std::endl;
                }
            }

            // acquire next frame
            VkAcquireNextImageInfoKHR acquireNextImageInfo {};
            acquireNextImageInfo.deviceMask = 1;
            acquireNextImageInfo.fence = VK_NULL_HANDLE;
            acquireNextImageInfo.pNext = VK_NULL_HANDLE;
            acquireNextImageInfo.semaphore = acquireSemaphore;
            acquireNextImageInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;
            acquireNextImageInfo.swapchain = swapChain->Handle();
            acquireNextImageInfo.timeout = UINT64_MAX;

            VkResult acquireNextImageResult = vkAcquireNextImage2KHR(device->Handle(), &acquireNextImageInfo, &imageIndex);
            //std::clog << "Image acquired: " << acquireNextImageResult << std::endl;
            if (
                acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR ||
                acquireNextImageResult == VK_SUBOPTIMAL_KHR ||
                framebufferResized
            ) {
                framebufferResized = false;

                std::cerr << "Swap chain image is out of date" << std::endl;
                continue;

                /*
                // recrete swap chain and dependent handles from scratch
                VkResult deviceWaitIdleResult = vkDeviceWaitIdle(device);
                if (deviceWaitIdleResult != VK_SUCCESS) {
                    std::cerr << "Unable to wait for idleing of the device (status: " << deviceWaitIdleResult << ")" << std::endl;
                    CleanOnExit(EXIT_FAILURE);
                }
                std::clog << "Device is now idle" << std::endl;            
                
                // destroy synchronization primitives
                for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                if (imageAvailableSemaphores[i] != VK_NULL_HANDLE) {
                    vkDestroySemaphore(device, imageAvailableSemaphores[i], VK_NULL_HANDLE);
                    std::clog << "'Image available' semaphore #" << i << " destroyed successfully" << std::endl;
                    imageAvailableSemaphores[i] = VK_NULL_HANDLE;
                }

                if (renderFinishedSemaphores[i] != VK_NULL_HANDLE) {
                    vkDestroySemaphore(device, renderFinishedSemaphores[i], VK_NULL_HANDLE);
                    std::clog << "'Render finished' semaphore #" << i << " destroyed successfully" << std::endl;
                    renderFinishedSemaphores[i] = VK_NULL_HANDLE;
                }

                if (inFlightFences[i] != VK_NULL_HANDLE) {
                    vkDestroyFence(device, inFlightFences[i], VK_NULL_HANDLE);
                    std::clog << "'In flight' fence #" << i << " destroyed successfully" << std::endl;
                    inFlightFences[i] = VK_NULL_HANDLE;
                }   
            }

                // destroy out of date frame buffers
                for (VkFramebuffer& frameBuffer : frameBuffers) {
                    if (frameBuffer != VK_NULL_HANDLE) {
                        vkDestroyFramebuffer(device, frameBuffer, VK_NULL_HANDLE);
                        std::clog << "Framebuffer destroyed successfully" << std::endl;
                        frameBuffer = VK_NULL_HANDLE;
                    }
                }

                // destroy out of date swap chain image views
                for (VkImageView& swapChainImageView : swapChainImageViews) {
                    if (swapChainImageView != VK_NULL_HANDLE) {
                        // destroy image view
                        vkDestroyImageView(device, swapChainImageView, VK_NULL_HANDLE);
                        std::clog << "Swap chain image view destroyed successfully" << std::endl;
                        swapChainImageView = VK_NULL_HANDLE;
                    }
                }
                
                // destroy out of date swap chain
                if (swapChain != VK_NULL_HANDLE) {
                    // destroy swapchain
                    vkDestroySwapchainKHR(device, swapChain, VK_NULL_HANDLE);
                    std::clog << "Swap chain destroyed successfully" << std::endl;
                    swapChain = VK_NULL_HANDLE;
                }
                
                int width = 0;
                int height = 0;
                if (!SDL_GetWindowSizeInPixels(window, &width, &height)) {
                    std::cerr << "Couldn't get window size: " << SDL_GetError() << std::endl;
                    swapchainExtent = choosenPhysicalDeviceSurfaceCapabilities.surfaceCapabilities.currentExtent;
                }

                VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
                };

                actualExtent.width  = std::clamp(actualExtent.width,
                                                choosenPhysicalDeviceSurfaceCapabilities.surfaceCapabilities.minImageExtent.width,
                                                choosenPhysicalDeviceSurfaceCapabilities.surfaceCapabilities.maxImageExtent.width);

                actualExtent.height = std::clamp(actualExtent.height,
                                                choosenPhysicalDeviceSurfaceCapabilities.surfaceCapabilities.minImageExtent.height,
                                                choosenPhysicalDeviceSurfaceCapabilities.surfaceCapabilities.maxImageExtent.height);
                
                swapchainExtent = actualExtent;

                std::cout << "ACTUAL EXTENT: " << actualExtent.width << "x" << actualExtent.height << std::endl;

                VkSwapchainCreateInfoKHR tmpSwapChainCreateInfo = swapChainCreateInfo;
                //tmpSwapChainCreateInfo.oldSwapchain = swapChain;
                createSwapChainResult = vkCreateSwapchainKHR(device, &swapChainCreateInfo, VK_NULL_HANDLE, &swapChain);
                if (createSwapChainResult != VK_SUCCESS) {
                    std::cerr << "Unable to create a swapChain (status: " << createSwapChainResult << ")" << std::endl;
                    return CleanOnExit(EXIT_FAILURE);
                }
                std::clog << "Swap chain created successully: <VkSwapchainKHR " << device << ">" << std::endl;

                // retrieve swap chain images
                swapChainImageCount = 0;
                getSwapChainImagesResult = vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, VK_NULL_HANDLE);
                if (getSwapChainImagesResult != VK_SUCCESS) {
                    std::cerr << "Unable to retrieve the swap chain images (1st call, status: " << getSwapChainImagesResult << ")" << std::endl;
                    return CleanOnExit(EXIT_FAILURE);
                }
                std::clog << "Swap chain images retrieved successully (1st call, count: " << swapChainImageCount << ")" << std::endl;

                swapChainImages = std::vector<VkImage>(swapChainImageCount);
                getSwapChainImagesResult = vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
                if (getSwapChainImagesResult != VK_SUCCESS) {
                    std::cerr << "Unable to retrieve the swap chain images (2nd call, status: " << getSwapChainImagesResult << ")" << std::endl;
                    return CleanOnExit(EXIT_FAILURE);
                }
                std::clog << "Swap chain images retrieved successully (2nd call, retrieved in array)" << std::endl;

                // create swap chain images views
                swapChainImageViews.resize(swapChainImages.size());

                for (size_t i = 0; i < swapChainImages.size(); ++i) {
                    // create swap chain image view
                    VkImageViewCreateInfo swapChainImageViewCreateInfo {};
                    swapChainImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                    swapChainImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                    swapChainImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                    swapChainImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                    swapChainImageViewCreateInfo.flags = 0;
                    swapChainImageViewCreateInfo.format = preferredFormat.surfaceFormat.format;
                    swapChainImageViewCreateInfo.image = swapChainImages[i];
                    swapChainImageViewCreateInfo.pNext = VK_NULL_HANDLE;
                    swapChainImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                    swapChainImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    swapChainImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
                    swapChainImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
                    swapChainImageViewCreateInfo.subresourceRange.layerCount = 1;
                    swapChainImageViewCreateInfo.subresourceRange.levelCount = 1;
                    swapChainImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

                    VkResult createImageViewResult = vkCreateImageView(device, &swapChainImageViewCreateInfo, VK_NULL_HANDLE, &swapChainImageViews[i]);
                    if (createImageViewResult != VK_SUCCESS) {
                        std::cerr << "Unable to create a swap chain image view (status: " << createImageViewResult << ")" << std::endl;
                        return CleanOnExit(EXIT_FAILURE);
                    }
                    std::clog << "Swap chain image view created successfully: <VkImageView " << swapChainImageViews[i] << ">" << std::endl;
                }

                // create frame buffers
                frameBuffers.resize(swapChainImageViews.size());
                for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
                    // create frame buffer
                    std::vector<VkImageView> attachments = {
                        swapChainImageViews[i]
                    };

                    VkFramebufferCreateInfo frameBufferCreateInfo {};
                    frameBufferCreateInfo.attachmentCount = 1;
                    frameBufferCreateInfo.flags = 0;
                    frameBufferCreateInfo.height = swapchainExtent.height;
                    frameBufferCreateInfo.layers = 1;
                    frameBufferCreateInfo.pAttachments = attachments.data();
                    frameBufferCreateInfo.pNext = VK_NULL_HANDLE;
                    frameBufferCreateInfo.renderPass = renderPass;
                    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    frameBufferCreateInfo.width = swapchainExtent.width;

                    VkResult createFramebufferResult = vkCreateFramebuffer(device, &frameBufferCreateInfo, VK_NULL_HANDLE, &frameBuffers[i]);
                    if (createFramebufferResult != VK_SUCCESS) {
                        std::cerr << "Could not create frame buffer (status: " << createFramebufferResult << ")" << std::endl;
                        CleanOnExit(EXIT_FAILURE);
                    }
                    std::clog << "Frame buffer created successfully: <VkFramebuffer " << frameBuffers[i] << ">" << std::endl;
                }

                for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                    VkResult createImageAvailableSemaphoreResult = vkCreateSemaphore(device, &semaphoreCreateInfo, VK_NULL_HANDLE, &imageAvailableSemaphores[i]);
                    if (createImageAvailableSemaphoreResult != VK_SUCCESS) {
                        std::cerr << "Unable to create 'Image available' semaphore (status: " << createImageAvailableSemaphoreResult << ")" << std::endl;
                        CleanOnExit(EXIT_FAILURE);
                    }
                    std::clog << "'Image available' semaphore created successfully: <VkSemaphore " << imageAvailableSemaphores[i] << ">" << std::endl;
                    
                    VkResult createRenderFinishedSemaphoreResult = vkCreateSemaphore(device, &semaphoreCreateInfo, VK_NULL_HANDLE, &renderFinishedSemaphores[i]);
                    if (createRenderFinishedSemaphoreResult != VK_SUCCESS) {
                        std::cerr << "Unable to create 'Render finished' semaphore (status: " << createRenderFinishedSemaphoreResult << ")" << std::endl;
                        CleanOnExit(EXIT_FAILURE);
                    }
                    std::clog << "'Render finished' semaphore created successfully: <VkSemaphore " << renderFinishedSemaphores[i] << ">" << std::endl;

                    VkResult createInFlightFenceResult = vkCreateFence(device, &fenceCreateInfo, VK_NULL_HANDLE, &inFlightFences[i]);
                    if (createInFlightFenceResult != VK_SUCCESS) {
                        std::cerr << "Unable to create 'In flight' fence (status: " << createInFlightFenceResult << ")" << std::endl;
                        CleanOnExit(EXIT_FAILURE);
                    }
                    std::clog << "'Render finished' fence created successfully: <VkFence " << inFlightFences[i] << ">" << std::endl;
                }
                */
            }

            VkResult resetFencesResult = vkResetFences(device->Handle(), 1, &frameFence);
            if (resetFencesResult != VK_SUCCESS) {
                std::cerr << "Could not reset fences (status: " << resetFencesResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            //std::clog << "Reset fences successfully" << std::endl;

            VkResult resetCommandBufferResult = vkResetCommandBuffer(commandBuffer, 0);
            if (resetCommandBufferResult != VK_SUCCESS) {
                std::cerr << "Could not reset command buffer (status: " << resetCommandBufferResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            //std::clog << "Reset command buffer successfully" << std::endl;

            // begin command buffer recording
            VkCommandBufferBeginInfo commandBufferBeginInfo {};
            commandBufferBeginInfo.flags = 0;
            commandBufferBeginInfo.pInheritanceInfo = VK_NULL_HANDLE;
            commandBufferBeginInfo.pNext = VK_NULL_HANDLE;
            commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            VkResult beginCommandBufferResult = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
            if (beginCommandBufferResult != VK_SUCCESS) {
                std::cerr << "Unable to begin command buffer recording (status: " << beginCommandBufferResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            //std::clog << "Command buffer recording began successfully: <VkCommandBuffer " << commandBuffer << ">" << std::endl;

            // begin render pass
            VkClearValue clearValue {};
            clearValue.color.float32[0] = 0.0f;
            clearValue.color.float32[1] = 0.0f;
            clearValue.color.float32[2] = 0.0f;
            clearValue.color.float32[3] = 1.0f;
            clearValue.depthStencil.depth = 0.0f;
            clearValue.depthStencil.stencil = 0;

            VkRenderPassBeginInfo renderPassBeginInfo {};
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.framebuffer = frameBuffers[frameIndex]->Handle();
            renderPassBeginInfo.pClearValues = &clearValue;
            renderPassBeginInfo.pNext = VK_NULL_HANDLE;
            renderPassBeginInfo.renderArea.extent = swapchainExtent;
            renderPassBeginInfo.renderArea.offset.x = 0;
            renderPassBeginInfo.renderArea.offset.y = 0;
            renderPassBeginInfo.renderPass = renderPass->Handle();
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

            VkSubpassBeginInfo subpassBeginInfo {};
            subpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;
            subpassBeginInfo.pNext = VK_NULL_HANDLE;
            subpassBeginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;

            vkCmdBeginRenderPass2(commandBuffer, &renderPassBeginInfo, &subpassBeginInfo);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->Handle());
        
            // set dynamic states
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport[0]);
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor[0]);

            // draw
            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

            // end render pass
            VkSubpassEndInfo subpassEndInfo {};
            subpassEndInfo.pNext = VK_NULL_HANDLE;
            subpassEndInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO;

            vkCmdEndRenderPass2(commandBuffer, &subpassEndInfo);

            VkResult endCommandBufferResult = vkEndCommandBuffer(commandBuffer);
            if (endCommandBufferResult != VK_SUCCESS) {
                std::cerr << "Unable to end the command buffer (status: " << endCommandBufferResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            //std::clog << "Command buffer ended successfully" << std::endl;
            
            // set command buffer for submission
            VkCommandBufferSubmitInfo commandBufferSubmitInfo {};
            commandBufferSubmitInfo.commandBuffer = commandBuffer;
            commandBufferSubmitInfo.deviceMask = 0;
            commandBufferSubmitInfo.pNext = VK_NULL_HANDLE;
            commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;

            // set synchronization handles behaviours for submission
            VkSemaphoreSubmitInfo waitSemaphoreSubmitInfo {};
            waitSemaphoreSubmitInfo.deviceIndex = 0;
            waitSemaphoreSubmitInfo.pNext = VK_NULL_HANDLE;
            waitSemaphoreSubmitInfo.semaphore = acquireSemaphore;
            waitSemaphoreSubmitInfo.stageMask = 0;
            waitSemaphoreSubmitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
            waitSemaphoreSubmitInfo.value = 0;

            std::vector<VkSemaphore> waitSemaphores = { acquireSemaphore };

            VkSemaphoreSubmitInfo signalSemaphoreSubmitInfo {};
            signalSemaphoreSubmitInfo.deviceIndex = 0;
            signalSemaphoreSubmitInfo.pNext = VK_NULL_HANDLE;
            signalSemaphoreSubmitInfo.semaphore = submitSemaphore;
            signalSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
            signalSemaphoreSubmitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
            signalSemaphoreSubmitInfo.value = 0;

            std::vector<VkSemaphore> signalSemaphores = { submitSemaphore };

            // submit command buffer to the queue
            VkSubmitInfo2 submitInfo {};
            submitInfo.commandBufferInfoCount = 1;
            submitInfo.flags = 0;
            submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
            submitInfo.pNext = VK_NULL_HANDLE;
            submitInfo.pSignalSemaphoreInfos = &signalSemaphoreSubmitInfo;
            submitInfo.pWaitSemaphoreInfos = &waitSemaphoreSubmitInfo;
            submitInfo.signalSemaphoreInfoCount = 1;
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
            submitInfo.waitSemaphoreInfoCount = 1;

            VkResult queueSubmitResult = vkQueueSubmit2(graphicsQueue->Handle(), 1, &submitInfo, frameFence);
            if (queueSubmitResult != VK_SUCCESS) {
                std::cerr << "Unable to submit to queue (status " << queueSubmitResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
        // std::clog << "Submitted to queue successfully" << std::endl;

            std::vector<VkSwapchainKHR> swapChains = { swapChain->Handle() };

            // present
            VkPresentInfoKHR presentInfo {};
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pNext = VK_NULL_HANDLE;
            presentInfo.pResults = VK_NULL_HANDLE;
            presentInfo.pSwapchains = swapChains.data();
            presentInfo.pWaitSemaphores = signalSemaphores.data();
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.swapchainCount = static_cast<uint32_t>(swapChains.size());
            presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());

            VkResult queuePresentResult = vkQueuePresentKHR(presentQueue->Handle(), &presentInfo);
            if (queuePresentResult != VK_SUCCESS) {
                std::cerr << "Unable to present image with queue (status: " << queuePresentResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            //std::clog << "Image presented" << std::endl;
            
            // cap frame index to MAX_FRAMES_IN_FLIGHT
            frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
        }

        // wait for device to be idle
        VkResult deviceWaitIdleResult = vkDeviceWaitIdle(device->Handle());
        if (deviceWaitIdleResult != VK_SUCCESS) {
            std::cerr << "Unable to wait for idleing of the device (status: " << deviceWaitIdleResult << ")" << std::endl;
            CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Device is now idle" << std::endl;
    }

    catch (std::exception const& err) {
        std::cerr << err.what() << std::endl;
    }

    return CleanOnExit(EXIT_SUCCESS);
}
