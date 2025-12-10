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

#include "VulkanHelpers/CreateInfos.hpp"
#include "VulkanHelpers/Flags.hpp"
#include "VulkanHelpers/Handles.hpp"
#include "VulkanHelpers/ParameterEnums.hpp"
#include "VulkanHelpers/ParameterInfos.hpp"
#include "VulkanHelpers/ParameterStructs.hpp"

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

/// TODO: VERY BAD IDEA HERE. Find a way to NOT give access to the handle (especially giving the right to create/destroy it)
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
) {
    device.WaitIdle();

    for (auto& framebuffer : framebuffers) {
        framebuffer.DestroyHandle();
    }

    for (auto& swapchainImageView : swapchainImageViews) {
        swapchainImageView.DestroyHandle();
    }

    swapchainImages.clear();

    swapchain.DestroyHandle();

    std::cout << swapchainCreateInfo.imageExtent.width << "x" << swapchainCreateInfo.imageExtent.height << std::endl;

    auto physicalDeviceSurfaceInfo = GeneratePhysicalDeviceSurfaceInfo(surface); // get surface info
    physicalDevice.SurfaceCapabilities(physicalDeviceSurfaceInfo);
    physicalDevice.SurfaceFormats(physicalDeviceSurfaceInfo); // get formats of the surface
    physicalDevice.PresentModes(surface); // get present modes of the surface

    swapchain.CreateHandle(swapchainCreateInfo);

    swapchainImages = EnumerateSwapChainImages(device, swapchain);

    for (int i = 0; i < static_cast<int>(swapchainImages.size()); ++i) {
        auto swapchainImageViewCreateInfo = GenerateImageViewCreateInfo(imageFormat, swapchainImages[i]);
        swapchainImageViews[i].CreateHandle(swapchainImageViewCreateInfo);
    }

    for (int i = 0; i < static_cast<int>(swapchainImageViews.size()); ++i) {
        std::vector<VkImageView> attachments = { swapchainImageViews[i].Handle() };
        auto frameBufferCreateInfo = GenerateFramebufferCreateInfo(swapchainCreateInfo.imageExtent, attachments, renderPass);
        framebuffers[i].CreateHandle(frameBufferCreateInfo);
    }
}

int main(int argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

    std::clog << "Vulkan header version: " << VK_HEADER_VERSION << std::endl;

    auto window = Window(800, 600);
#ifndef NDEBUG
    bool enableValidationLayers = true;
#else
    bool enableValidationLayers = false;
#endif
    constexpr int MAX_FRAMES_IN_FLIGHT = 4;
    uint frames_counter = 0;

    auto CleanOnExit = [&](int code) {
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
        /*
        std::clog << "Instance layers properties:" << std::endl;
        std::clog << instanceLayersProperties << std::endl; // display instance layers properties 
        */

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

        //enabledExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // for MacOS, to avoid VK_ERROR_INCOMPATIBLE_DRIVER on vkCreateInstance
        enabledExtensions.emplace_back("VK_KHR_get_surface_capabilities2");
        if (enableValidationLayers) {
            enabledExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        /*
        std::clog << "Enabled extensions count retrieved successfully from SDL_Vulkan_GetInstanceExtensions (count: " << enabledExtensions.size() << ")" << std::endl;
        std::clog << "Enabled extensions:" << std::endl;
        for (const char* enabledExtension : enabledExtensions) {
            std::clog << " - " << enabledExtension << std::endl;
        }
        */


        auto instanceExtensionsProperties = EnumerateInstanceExtensionProperties();

        /*
        std::clog << "Instance extensions:" << std::endl;
        std::clog << instanceExtensionsProperties << std::endl;
        */

        DisplaySupportedExtensionsFromSDLVulkan(enabledExtensions, instanceExtensionsProperties);

    #ifndef NDEBUG
        VkInstanceCreateInfo instanceCreateInfo {};
        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo {};
        if (enableValidationLayers) {
            debugUtilsMessengerCreateInfo = GenerateDebugUtilsMessengerCreateInfo(DebugCallback);
            instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR, &debugUtilsMessengerCreateInfo);
        }

        else  {
            instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
        }
        
        auto instance = Instance(instanceCreateInfo); // create instance
        
        // a pointer to nullptr by default since validation layers may not be enabled so debug utils will not be enabled as well (by default)
        std::unique_ptr<DebugUtilsMessenger> debugUtilsMessenger = nullptr;
        if (enableValidationLayers) {
            debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(instance, debugUtilsMessengerCreateInfo); // create the global debug messenger
        }
    #else
        //auto instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
        auto instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers);
        auto instance = Instance(instanceCreateInfo); // create instance
    #endif

        auto surface = Surface(instance, window); // create the surface from SDL

        auto enumeratedPhysicalDevices = PhysicalDevice::Enumerate(instance); // enumerate physical devices

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
        VkSurfaceCapabilities2KHR choosenPhysicalDeviceSurfaceCapabilities {};
        std::optional<uint32_t> queueFamilyIndexWithGraphicsCapabilities = std::nullopt;
        std::optional<uint32_t> queueFamilyIndexWithPresentCapabilities = std::nullopt;

        std::unique_ptr<PhysicalDevice> physicalDevice = nullptr;

        // enumerate each physical device's properties
        std::ios_base::fmtflags formatFlags = std::clog.flags();
        for (PhysicalDevice const& enumeratedPhysicalDevice : enumeratedPhysicalDevices) {
            //std::clog << "Physical device found: <VkPhysicalDevice " << enumeratedPhysicalDevice.Handle() << ">" << std::endl;
            
            auto physicalDeviceProperties = GeneratePhysicalDeviceProperties(enumeratedPhysicalDevice); // get physical device's properties
            auto physicalDeviceFeatures = GeneratePhysicalDeviceFeatures(enumeratedPhysicalDevice); // get physical device's features
            auto physicalDeviceQueueFamilyProperties = EnumerateQueueFamilyProperties(enumeratedPhysicalDevice); // get physical device's queue family properties
        
            int queueFamilyIndex = 0;
            //std::clog << " - Queue families (count: " << physicalDeviceQueueFamilyProperties.size() << ")" << std::endl;
            for (VkQueueFamilyProperties2 const& queueFamilyProperties : physicalDeviceQueueFamilyProperties) {
                if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    queueFamilyIndexWithGraphicsCapabilities = queueFamilyIndex; 
                }

                // check that the retrieved surface is supported by a specific queue family for the current physical device
                VkBool32 supportedSurface = enumeratedPhysicalDevice.IsSurfaceSupportedByQueueFamily(surface, queueFamilyIndex);
                if (supportedSurface) {
                    queueFamilyIndexWithPresentCapabilities = queueFamilyIndex;
                }
            }

            // enumerate device extensions
            auto deviceExtensionsProperties = EnumerateDeviceExtensionProperties(enumeratedPhysicalDevice); // enumerate device extensions
            bool deviceExtensionsSupported = AreDeviceExtensionsSupported(deviceExtensions, deviceExtensionsProperties);
            //std::clog << (deviceExtensionsSupported ? "All device extensions are supported" : "Some or all device extensions aren't supported") << std::endl;

            auto physicalDeviceSurfaceInfo = GeneratePhysicalDeviceSurfaceInfo(surface); // get surface info
            auto surfaceCapabilities = enumeratedPhysicalDevice.SurfaceCapabilities(physicalDeviceSurfaceInfo); // get capabilities of the surface
            auto surfaceFormats = enumeratedPhysicalDevice.SurfaceFormats(physicalDeviceSurfaceInfo); // get formats of the surface
            auto presentModes = enumeratedPhysicalDevice.PresentModes(surface); // get present modes of the surface

            // check is swapchain has mandatory properties
            bool swapchainIsAdequate = false;
            if (deviceExtensionsSupported) {
                swapchainIsAdequate = !surfaceFormats.empty() && !presentModes.empty();
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
            deviceName.find("Microsoft Direct3D") == std::string::npos
            && physicalDeviceFeatures.features.geometryShader
            && queueFamilyIndexWithGraphicsCapabilities.has_value()
            && queueFamilyIndexWithPresentCapabilities.has_value()
            && deviceExtensionsSupported
            && swapchainIsAdequate
            ) {
                std::clog << "Physical device selected successfully: <VkPhysicalDevice " << enumeratedPhysicalDevice.Handle() << ">" << std::endl;
                //std::clog << "Current physical device selected" << std::endl;
                choosenPhysicalDeviceSurfaceCapabilities = surfaceCapabilities;
                physicalDevice = std::make_unique<PhysicalDevice>(enumeratedPhysicalDevice);
            }
        }
        std::clog.flags(formatFlags);

        // check if a suitable device was found
        if (physicalDevice == VK_NULL_HANDLE || !queueFamilyIndexWithGraphicsCapabilities.has_value() || !queueFamilyIndexWithPresentCapabilities.has_value()) {
            std::cerr << "Unable to get a suitable physical device" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }

        /// TODO: IMPLEMENT THESE LAMBDAS CORRECTLY TO BE USED AS PREDICATE FOR FUTURE IMPLEMENTATION OF ISSUITABLE()
        // user defined so that the user can choose specifically which format to use
        //auto selectPreferredSurfaceFormat = [&physicalDevice, &surface]() -> bool {
        //    return true;
        //};

        // user defined so that the user can choose specifically which present mode to use
        //auto selectPreferredPresentMode = [&physicalDevice, &surface]() -> bool {
        //    return true;
        //};

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
        auto device = Device(deviceCreateInfo, physicalDevice->Handle()); // create device
    
        auto deviceGraphicsQueueInfo = GenerateDeviceQueueInfo(0, queueFamilyIndexWithGraphicsCapabilities.value());
        auto graphicsQueue = Queue(deviceGraphicsQueueInfo, device); // get device graphics queue

        VkDeviceQueueInfo2 devicePresentQueueInfo = GenerateDeviceQueueInfo(0, queueFamilyIndexWithPresentCapabilities.value());
        auto presentQueue = Queue(devicePresentQueueInfo, device); // get device present queue

        auto swapchainCreateInfo = GenerateSwapchainCreateInfo(
            preferredFormat.surfaceFormat.colorSpace,
            swapchainExtent,
            preferredFormat.surfaceFormat.format,
            queueFamilyIndexWithGraphicsCapabilities.value(),
            queueFamilyIndexWithPresentCapabilities.value(),
            imageCount,
            preferredPresentMode,
            currentTransform,
            surface
        );
        auto swapchain = Swapchain(swapchainCreateInfo, device); // create swapchain

        std::vector<VkImage> swapchainImages = EnumerateSwapChainImages(device, swapchain); // retrieve swap chain images

        std::vector<ImageView> swapchainImageViews;
        for (size_t i = 0; i < swapchainImages.size(); ++i) {
            auto swapchainImageViewCreateInfo = GenerateImageViewCreateInfo(preferredFormat.surfaceFormat.format, swapchainImages[i]);
            swapchainImageViews.emplace_back(swapchainImageViewCreateInfo, device); // create swap chain image view
        }
        
        std::vector<char> vertexShaderFileBuffer;
        auto vertexShaderModuleCreateInfo = GenerateShaderModuleCreateInfo("resources/shaders/triangle.vertex.spv", vertexShaderFileBuffer);
        auto vertexShaderModule = ShaderModule(vertexShaderModuleCreateInfo, device); // create vertex shader module

        std::vector<char> fragmentShaderFileBuffer;
        auto fragmentShaderModuleCreateInfo = GenerateShaderModuleCreateInfo("resources/shaders/triangle.fragment.spv", fragmentShaderFileBuffer);
        auto fragmentShaderModule = ShaderModule(fragmentShaderModuleCreateInfo, device); // create fragment shader module
        
        std::string shaderMainFunctionName = "main";
        auto vertexShaderPipelineStageCreateInfo = GeneratePipelineShaderStageCreateInfo(vertexShaderModule, shaderMainFunctionName, VK_SHADER_STAGE_VERTEX_BIT); // create vertex shader pipeline stage
        auto fragmentShaderPipelineStageCreateInfo = GeneratePipelineShaderStageCreateInfo(fragmentShaderModule, shaderMainFunctionName, VK_SHADER_STAGE_FRAGMENT_BIT); // create fragment shader pipeline stage

        std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos = { vertexShaderPipelineStageCreateInfo, fragmentShaderPipelineStageCreateInfo };
        std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        auto pipelineDynamicStateCreateInfo = GeneratePipelineDynamicStateCreateInfo(dynamicStates); // specify pipeline dynamic states

        auto pipelineVertexInputStateCreateInfo = GeneratePipelineVertexInputStateCreateInfo(); // specify pipeline vertex inputs
        auto pipelineInputAssemblyStateCreateInfo = GeneratePipelineInputAssemblyStateCreateInfo(); // specify pipeline input assemblies
        
        std::vector<VkViewport> viewport = { GenerateViewport(static_cast<float>(swapchainExtent.width), static_cast<float>(swapchainExtent.height)) }; // specify viewport
        std::vector<VkRect2D> scissor = { GenerateRect2D(swapchainExtent) }; // specify scissor
        auto pipelineViewportStateCreateInfo = GeneratePipelineViewportStateCreateInfo(viewport, scissor);

        auto pipelineRasterizationStateCreateInfo = GeneratePipelineRasterizationStateCreateInfo(); // specify pipeline rasterization state
        auto pipelineMultisampleStateCreateInfo = GeneratePipelineMultisampleStateCreateInfo(); // specify pipeline multisampling state
        //auto pipelineDepthStencilStateCreateInfo = GeneratePipelineDepthStencilStateCreateInfo(); // specify pipeline depth stencil state

        std::vector<VkPipelineColorBlendAttachmentState> pipelineColorBlendAttachmentState =  { GeneratePipelineColorBlendAttachmentState() }; // specify pipeline color blend attachment state
        auto pipelineColorBlendStateCreateInfo = GeneratePipelineColorBlendStateCreateInfo(pipelineColorBlendAttachmentState);

        auto pipelineLayoutCreateInfo = GeneratePipelineLayoutCreateInfo(); // create pipeline layout
        auto pipelineLayout = PipelineLayout(pipelineLayoutCreateInfo, device);

        std::vector<VkAttachmentDescription2> attachmentDescription = { GenerateAttachmentDescription(preferredFormat.surfaceFormat.format) }; // specify attachment description
        std::vector<VkAttachmentReference2> attachmentReference = { GenerateAttachmentReference() }; // create attachment reference
        std::vector<VkSubpassDescription2> subpassDescription = { GenerateSubpassDescription(attachmentReference) }; // specify subpass descriptions
        std::vector<VkSubpassDependency2> subpassDependency = { GenerateSubpassDependency() }; // specify subpass dependencies
        auto renderPassCreateInfo = GenerateRenderPassCreateInfo(attachmentDescription, subpassDependency, subpassDescription);
        auto renderPass = RenderPass(renderPassCreateInfo, device);

        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = GenerateGraphicsPipelineCreateInfo(
            pipelineDynamicStateCreateInfo,
            pipelineVertexInputStateCreateInfo,
            pipelineInputAssemblyStateCreateInfo,
            pipelineViewportStateCreateInfo,
            pipelineRasterizationStateCreateInfo,
            pipelineMultisampleStateCreateInfo,
            pipelineColorBlendStateCreateInfo,
            pipelineLayout,
            pipelineShaderStageCreateInfos,
            renderPass
        );
        auto graphicsPipeline = Pipeline(graphicsPipelineCreateInfo, device); // create graphics pipeline

        // create frame buffers
        std::vector<Framebuffer> framebuffers;
        for (int i = 0; i < static_cast<int>(swapchainImageViews.size()); ++i) {
            std::vector<VkImageView> attachments = { swapchainImageViews[i].Handle() };
            auto frameBufferCreateInfo = GenerateFramebufferCreateInfo(swapchainExtent, attachments, renderPass);
            framebuffers.emplace_back(frameBufferCreateInfo, device); // create frame buffer
        }

        auto commandPoolCreateInfo = GenerateCommandPoolCreateInfo(queueFamilyIndexWithGraphicsCapabilities.value());
        auto commandPool = CommandPool(commandPoolCreateInfo, device); // create command pool

        auto commandBuffers = std::vector<CommandBuffer>(MAX_FRAMES_IN_FLIGHT);
        auto commandBufferAllocateInfo = GenerateCommandBufferAllocateInfo(commandPool, static_cast<uint32_t>(commandBuffers.size())); // create command buffer
        commandBuffers = AllocateCommandBuffers(commandBufferAllocateInfo, device);

        // create synchronization primitive objects
        auto semaphoreCreateInfo = GenerateSemaphoreCreateInfo();
        auto fenceCreateInfo = GenerateFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT); // signaled on creation to avoid deadlock on the first frame

        std::vector<Semaphore> imageAvailableSemaphores {};
        std::vector<Fence> inFlightFences {};
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            imageAvailableSemaphores.emplace_back(semaphoreCreateInfo, device);
            inFlightFences.emplace_back(fenceCreateInfo, device);
        }

        std::vector<Semaphore> renderFinishedSemaphores {};
        for (int i = 0; i < static_cast<int>(swapchainImages.size()); ++i) {
            renderFinishedSemaphores.emplace_back(semaphoreCreateInfo, device);
        }

        // main loop variables
        SDL_Event event {};
        bool running = true;
        uint32_t frameIndex = 0;
        bool framebufferResized = false;
        uint last_tick_number = SDL_GetTicks();
        while (running)
        {
            frames_counter++;

            if (SDL_GetTicks() > last_tick_number + 1000) {
                std::clog << "FPS: " << frames_counter << std::endl;
                frames_counter = 0;
                last_tick_number = SDL_GetTicks();
            }

            Fence& frameFence = inFlightFences[frameIndex];
            std::vector<VkFence> fencesToResetAndWaitFor = { frameFence.Handle() };
            WaitForFences(device, fencesToResetAndWaitFor);

            uint32_t imageIndex = 0;
            Semaphore& acquireSemaphore = imageAvailableSemaphores[frameIndex];
            CommandBuffer& commandBuffer = commandBuffers[frameIndex];

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                }

                if (event.type == SDL_EVENT_WINDOW_RESIZED ) {
                    framebufferResized = true;
                    std::clog << "Window resized to " << event.window.data1 << "x" << event.window.data2 << std::endl;
                    swapchainCreateInfo.imageExtent.width = event.window.data1;
                    swapchainCreateInfo.imageExtent.height = event.window.data2;
                }
            }

            auto acquireNextImageInfo = GenerateAcquireNextImageInfo(swapchain, &acquireSemaphore);
            VkResult acquireNextImageResult = vkAcquireNextImage2KHR(device.Handle(), &acquireNextImageInfo, &imageIndex); // acquire next frame
            if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
                std::cerr << "Swap chain image is out of date" << std::endl;

                HandleFrameInvalidity(*physicalDevice, device, surface, swapchainCreateInfo, swapchain, preferredFormat.surfaceFormat.format, swapchainImages, swapchainImageViews, framebuffers, renderPass);
                continue;
            }

            else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
                throw std::runtime_error("Could not aquire swapchain image");
            }

            ResetFences(device, fencesToResetAndWaitFor);
            
            // REALLY IMPORTANT: USE SWAPCHAIN IMAGE INDEX RETRIEVED FROM ANI TO PROPERLY RE-USE SEMAPHORES
            Semaphore& submitSemaphore = renderFinishedSemaphores[imageIndex];

            commandBuffer.Reset();
 
            auto commandBufferBeginInfo = GenerateCommandBufferBeginInfo();
            commandBuffer.Begin(commandBufferBeginInfo); // begin command buffer recording

            std::vector<VkClearValue> clearValue = { GenerateClearValue() };
            auto renderPassBeginInfoRenderArea = GenerateRect2D(swapchainExtent);
            VkRenderPassBeginInfo renderPassBeginInfo = GenerateRenderPassBeginInfo(renderPass, framebuffers[frameIndex], clearValue, renderPassBeginInfoRenderArea);
            auto subpassBeginInfo = GenerateSubpassBeginInfo();
            commandBuffer.BeginRenderPass(renderPassBeginInfo, subpassBeginInfo); // begin render pass

            commandBuffer.BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        
            // set dynamic states
            commandBuffer.SetViewport(0, 1, viewport);
            commandBuffer.SetScissor(0, 1, scissor);

            commandBuffer.Draw(3, 1, 0, 0);

            auto subpassEndInfo = GenerateSubpassEndInfo();
            commandBuffer.EndRenderPass(subpassEndInfo); // end render pass

            commandBuffer.End();
            
            std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfo = { GenerateCommandBufferSubmitInfo(commandBuffer) }; // specify command buffer submit info for submission
            std::vector<VkSemaphoreSubmitInfo> signalSemaphoreSubmitInfo = { GenerateSemaphoreSubmitInfo(submitSemaphore, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT) };
            std::vector<VkSemaphore> signalSemaphores = { submitSemaphore.Handle() };
            std::vector<VkSemaphoreSubmitInfo> waitSemaphoreSubmitInfo = { GenerateSemaphoreSubmitInfo(acquireSemaphore) }; // set synchronization handles behaviours for submission
            std::vector<VkSemaphore> waitSemaphores = { acquireSemaphore.Handle() };

            VkSubmitInfo2 submitInfo = GenerateSumbitInfo(commandBufferSubmitInfo, signalSemaphoreSubmitInfo, waitSemaphoreSubmitInfo);
            VkResult queueSubmitResult = vkQueueSubmit2(graphicsQueue.Handle(), 1, &submitInfo, frameFence.Handle()); // submit command buffer to the queue
            if (queueSubmitResult != VK_SUCCESS) {
                std::cerr << "Unable to submit to queue (status " << queueSubmitResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }

            std::vector<uint32_t> imageIndices = { imageIndex };
            std::vector<VkSwapchainKHR> swapchains = { swapchain.Handle() };

            VkPresentInfoKHR presentInfo = GeneratePresentInfo(imageIndices, swapchains, signalSemaphores);
            VkResult queuePresentResult = vkQueuePresentKHR(presentQueue.Handle(), &presentInfo); // present
            if (queuePresentResult != VK_SUCCESS) {
                std::cerr << "Unable to present image with queue (status: " << queuePresentResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
            }
            
            if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR || framebufferResized) {
                framebufferResized = false;
                HandleFrameInvalidity(*physicalDevice, device, surface, swapchainCreateInfo, swapchain, preferredFormat.surfaceFormat.format, swapchainImages, swapchainImageViews, framebuffers, renderPass);
            }
            
            else if (queuePresentResult != VK_SUCCESS) {
                throw std::runtime_error("Could not present swap chain image");
            }

            // cap frame index to MAX_FRAMES_IN_FLIGHT
            frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
        }

        device.WaitIdle(); // wait for device to be idle
    }

    catch (std::exception const& err) {
        std::cerr << err.what() << std::endl;
    }

    return CleanOnExit(EXIT_SUCCESS);
}

/// to move back at the correct place once possible 
/*
// recrete swap chain and dependent handles from scratch
VkResult deviceWaitIdleResult = vkDeviceWaitIdle(device);
if (deviceWaitIdleResult != VK_SUCCESS) {
    std::cerr << "Unable to wait for idleing of the device (status: " << deviceWaitIdleResult << ")" << std::endl;
    CleanOnExit(EXIT_FAILURE);
}
std::clog << "Device is now idle" << std::endl;            

// destroy out of date frame buffers
for (VkFramebuffer& frameBuffer : framebuffers) {
    if (frameBuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(device, frameBuffer, VK_NULL_HANDLE);
        std::clog << "Framebuffer destroyed successfully" << std::endl;
        frameBuffer = VK_NULL_HANDLE;
    }
}

// destroy out of date swap chain image views
for (VkImageView& swapchainImageView : swapchainImageViews) {
    if (swapchainImageView != VK_NULL_HANDLE) {
        // destroy image view
        vkDestroyImageView(device, swapchainImageView, VK_NULL_HANDLE);
        std::clog << "Swap chain image view destroyed successfully" << std::endl;
        swapchainImageView = VK_NULL_HANDLE;
    }
}

// destroy out of date swap chain
if (swapchain != VK_NULL_HANDLE) {
    // destroy swapchain
    vkDestroySwapchainKHR(device, swapchain, VK_NULL_HANDLE);
    std::clog << "Swap chain destroyed successfully" << std::endl;
    swapchain = VK_NULL_HANDLE;
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

VkSwapchainCreateInfoKHR tmpSwapChainCreateInfo = swapchainCreateInfo;
//tmpSwapChainCreateInfo.oldSwapchain = swapchain;
createSwapChainResult = vkCreateSwapchainKHR(device, &swapchainCreateInfo, VK_NULL_HANDLE, &swapchain);
if (createSwapChainResult != VK_SUCCESS) {
    std::cerr << "Unable to create a swapchain (status: " << createSwapChainResult << ")" << std::endl;
    return CleanOnExit(EXIT_FAILURE);
}
std::clog << "Swap chain created successully: <VkSwapchainKHR " << device << ">" << std::endl;

// retrieve swap chain images
swapchainImageCount = 0;
getSwapChainImagesResult = vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, VK_NULL_HANDLE);
if (getSwapChainImagesResult != VK_SUCCESS) {
    std::cerr << "Unable to retrieve the swap chain images (1st call, status: " << getSwapChainImagesResult << ")" << std::endl;
    return CleanOnExit(EXIT_FAILURE);
}
std::clog << "Swap chain images retrieved successully (1st call, count: " << swapchainImageCount << ")" << std::endl;

swapchainImages = std::vector<VkImage>(swapchainImageCount);
getSwapChainImagesResult = vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());
if (getSwapChainImagesResult != VK_SUCCESS) {
    std::cerr << "Unable to retrieve the swap chain images (2nd call, status: " << getSwapChainImagesResult << ")" << std::endl;
    return CleanOnExit(EXIT_FAILURE);
}
std::clog << "Swap chain images retrieved successully (2nd call, retrieved in array)" << std::endl;

// create swap chain images views
swapchainImageViews.resize(swapchainImages.size());

for (size_t i = 0; i < swapchainImages.size(); ++i) {
    // create swap chain image view
    VkImageViewCreateInfo swapchainImageViewCreateInfo {};
    swapchainImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    swapchainImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    swapchainImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    swapchainImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    swapchainImageViewCreateInfo.flags = 0;
    swapchainImageViewCreateInfo.format = preferredFormat.surfaceFormat.format;
    swapchainImageViewCreateInfo.image = swapchainImages[i];
    swapchainImageViewCreateInfo.pNext = VK_NULL_HANDLE;
    swapchainImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    swapchainImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    swapchainImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    swapchainImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    swapchainImageViewCreateInfo.subresourceRange.layerCount = 1;
    swapchainImageViewCreateInfo.subresourceRange.levelCount = 1;
    swapchainImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

    VkResult createImageViewResult = vkCreateImageView(device, &swapchainImageViewCreateInfo, VK_NULL_HANDLE, &swapchainImageViews[i]);
    if (createImageViewResult != VK_SUCCESS) {
        std::cerr << "Unable to create a swap chain image view (status: " << createImageViewResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Swap chain image view created successfully: <VkImageView " << swapchainImageViews[i] << ">" << std::endl;
}

// create frame buffers
framebuffers.resize(swapchainImageViews.size());
for (size_t i = 0; i < swapchainImageViews.size(); ++i) {
    // create frame buffer
    std::vector<VkImageView> attachments = {
        swapchainImageViews[i]
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

    VkResult createFramebufferResult = vkCreateFramebuffer(device, &frameBufferCreateInfo, VK_NULL_HANDLE, &framebuffers[i]);
    if (createFramebufferResult != VK_SUCCESS) {
        std::cerr << "Could not create frame buffer (status: " << createFramebufferResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Frame buffer created successfully: <VkFramebuffer " << framebuffers[i] << ">" << std::endl;
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
