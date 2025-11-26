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

int main(int argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

    std::clog << "Vulkan header version: " << VK_HEADER_VERSION << std::endl;

    Window window;
#ifndef NDEBUG
    bool enableValidationLayers = true;
#else
    bool enableValidationLayers = false;
#endif
    constexpr int MAX_FRAMES_IN_FLIGHT = 2;

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

        enabledExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // for MacOS, to avoid VK_ERROR_INCOMPATIBLE_DRIVER on vkCreateInstance
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
        auto instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
        auto instance = Instance(instanceCreateInfo); // create instance
    #endif

        auto surface = Surface(instance, window); // create the surface from SDL

        std::vector<PhysicalDevice> enumeratedPhysicalDevices = PhysicalDevice::Enumerate(instance); // enumerate physical devices

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

                // check that the retrieved surface is supported by a specific queue family
                VkBool32 supportedSurface = surface.IsSupportedByQueueFamily(enumeratedPhysicalDevice, queueFamilyIndex);
                if (supportedSurface) {
                    queueFamilyIndexWithPresentCapabilities = queueFamilyIndex;
                }
            }

            // enumerate device extensions
            auto deviceExtensionsProperties = EnumerateDeviceExtensionProperties(enumeratedPhysicalDevice); // enumerate device extensions
            bool deviceExtensionsSupported = AreDeviceExtensionsSupported(deviceExtensions, deviceExtensionsProperties);
            //std::clog << (deviceExtensionsSupported ? "All device extensions are supported" : "Some or all device extensions aren't supported") << std::endl;

            auto physicalDeviceSurfaceInfo = GeneratePhysicalDeviceSurfaceInfo(surface); // get surface info
            auto surfaceCapabilities = GenerateSurfaceCapabilities(enumeratedPhysicalDevice, physicalDeviceSurfaceInfo); // get capabilities of the surface
            auto surfaceFormats = EnumerateSurfaceFormats(enumeratedPhysicalDevice, physicalDeviceSurfaceInfo); // get formats of the surface
            auto presentModes = EnumeratePresentModes(enumeratedPhysicalDevice, surface); // get present modes of the surface

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

        auto swapChainCreateInfo = GenerateSwapchainCreateInfo(
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
        auto swapChain = Swapchain(swapChainCreateInfo, device); // create swapchain

        std::vector<VkImage> swapChainImages = EnumerateSwapChainImages(device, swapChain); // retrieve swap chain images

        std::vector<ImageView> swapChainImageViews;
        for (size_t i = 0; i < swapChainImages.size(); ++i) {
            auto swapChainImageViewCreateInfo = GenerateImageViewCreateInfo(preferredFormat.surfaceFormat.format, swapChainImages[i]);
            swapChainImageViews.emplace_back(swapChainImageViewCreateInfo, device); // create swap chain image view
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
        std::vector<Framebuffer> frameBuffers;
        for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
            std::vector<VkImageView> attachments = { swapChainImageViews[i].Handle() };
            auto frameBufferCreateInfo = GenerateFramebufferCreateInfo(swapchainExtent, attachments, renderPass);
            frameBuffers.emplace_back(frameBufferCreateInfo, device); // create frame buffer
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
        for (int i = 0; i < swapChainImages.size(); ++i) {
            renderFinishedSemaphores.emplace_back(semaphoreCreateInfo, device);
        }

        // main loop variables
        SDL_Event event {};
        bool running = true;
        uint32_t frameIndex = 0;
        bool framebufferResized = false;

        while (running) {
            Fence& frameFence = inFlightFences[frameIndex];
            std::vector<VkFence> fencesToResetAndWaitFor = { frameFence.Handle() };
            WaitForFences(device, fencesToResetAndWaitFor);
            ResetFences(device, fencesToResetAndWaitFor);
            
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                }

                if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                    framebufferResized = true;
                    std::clog << "Window resized to " << event.window.data1 << "x" << event.window.data2 << std::endl;
                }
            }

            uint32_t imageIndex = 0;
            Semaphore& acquireSemaphore = imageAvailableSemaphores[frameIndex];
            CommandBuffer& commandBuffer = commandBuffers[frameIndex];
        
            auto acquireNextImageInfo = GenerateAcquireNextImageInfo(swapChain, &acquireSemaphore);
            VkResult acquireNextImageResult = vkAcquireNextImage2KHR(device.Handle(), &acquireNextImageInfo, &imageIndex); // acquire next frame
            if (
                acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR ||
                acquireNextImageResult == VK_SUBOPTIMAL_KHR ||
                framebufferResized
            ) {
                framebufferResized = false;

                std::cerr << "Swap chain image is out of date" << std::endl;
                continue;

                /// TODO: CODE AT THE END OF THIS FILE IS SUPPOSED TO BE HERE
            }

            Semaphore& submitSemaphore = renderFinishedSemaphores[imageIndex];

            commandBuffer.Reset();
 
            auto commandBufferBeginInfo = GenerateCommandBufferBeginInfo();
            commandBuffer.Begin(commandBufferBeginInfo); // begin command buffer recording

            std::vector<VkClearValue> clearValue = { GenerateClearValue() };
            auto renderPassBeginInfoRenderArea = GenerateRect2D(swapchainExtent);
            VkRenderPassBeginInfo renderPassBeginInfo = GenerateRenderPassBeginInfo(renderPass, frameBuffers[frameIndex], clearValue, renderPassBeginInfoRenderArea);
            auto subpassBeginInfo = GenerateSubpassBeginInfo();
            commandBuffer.BeginRenderPass(renderPassBeginInfo, subpassBeginInfo); // begin render pass

            commandBuffer.BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        
            // set dynamic states
            commandBuffer.SetViewport(0, 1, viewport);
            commandBuffer.SetScissor(0, 1, scissor);

            commandBuffer.Draw(6, 1, 0, 0);

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
            std::vector<VkSwapchainKHR> swapChains = { swapChain.Handle() };

            VkPresentInfoKHR presentInfo = GeneratePresentInfo(imageIndices, swapChains, signalSemaphores);
            VkResult queuePresentResult = vkQueuePresentKHR(presentQueue.Handle(), &presentInfo); // present
            if (queuePresentResult != VK_SUCCESS) {
                std::cerr << "Unable to present image with queue (status: " << queuePresentResult << ")" << std::endl;
                CleanOnExit(EXIT_FAILURE);
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