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

#include "Application.hpp"

int main(int argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

    std::clog << "Vulkan header version: " << VK_HEADER_VERSION << std::endl;

    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 800;

    auto window = Window(WINDOW_WIDTH, WINDOW_HEIGHT);
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
            instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, 0 /* VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR */, &debugUtilsMessengerCreateInfo);
        }

        else  {
            instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, 0 /* VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR */);
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
        std::optional<uint32_t> queueFamilyIndexWithTransferCapabilities = std::nullopt;

        std::unique_ptr<PhysicalDevice> physicalDevice = nullptr;

        // enumerate each physical device's properties
        std::ios_base::fmtflags formatFlags = std::clog.flags();
        for (PhysicalDevice const& enumeratedPhysicalDevice : enumeratedPhysicalDevices) {
            //std::clog << "Physical device found: <VkPhysicalDevice " << enumeratedPhysicalDevice.Handle() << ">" << std::endl;
            
            auto physicalDeviceProperties = enumeratedPhysicalDevice.Properties(); // get physical device's properties
            auto physicalDeviceFeatures = enumeratedPhysicalDevice.Features(); // get physical device's features
            auto physicalDeviceQueueFamilyProperties = enumeratedPhysicalDevice.QueueFamilyProperties(); // get physical device's queue family properties

            int queueFamilyIndex = 0;
            //std::clog << " - Queue families (count: " << physicalDeviceQueueFamilyProperties.size() << ", "  << physicalDeviceProperties.properties.deviceName << ")" << std::endl;
            for (VkQueueFamilyProperties2 const& queueFamilyProperties : physicalDeviceQueueFamilyProperties) {
                if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    queueFamilyIndexWithGraphicsCapabilities = queueFamilyIndex; 
                }

                if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                    queueFamilyIndexWithTransferCapabilities = queueFamilyIndex;
                }

                // check that the retrieved surface is supported by a specific queue family for the current physical device
                bool supportedSurface = enumeratedPhysicalDevice.IsSurfaceSupportedByQueueFamily(surface, queueFamilyIndex);
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
            && physicalDeviceProperties.properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && physicalDeviceFeatures.features.geometryShader
            && queueFamilyIndexWithGraphicsCapabilities.has_value()
            && queueFamilyIndexWithPresentCapabilities.has_value()
            && deviceExtensionsSupported
            && swapchainIsAdequate
            ) {
                std::clog << "Physical device selected successfully: <VkPhysicalDevice " << enumeratedPhysicalDevice.Handle() << ">" << std::endl;
                std::clog << "Current physical device selected: " << physicalDeviceProperties.properties.deviceName << std::endl;
                choosenPhysicalDeviceSurfaceCapabilities = surfaceCapabilities;
                physicalDevice = std::make_unique<PhysicalDevice>(enumeratedPhysicalDevice);
                if (queueFamilyIndexWithGraphicsCapabilities.has_value()) { std::cout << "Graphics queue family index: " << queueFamilyIndexWithGraphicsCapabilities.value() << std::endl; }
                if (queueFamilyIndexWithPresentCapabilities.has_value()) { std::cout << "Present queue family index: " << queueFamilyIndexWithPresentCapabilities.value() << std::endl; }
                //if (queueFamilyIndexWithTransferCapabilitiesOnly.has_value()) { std::cout << "Transfer only queue family index: " << queueFamilyIndexWithTransferCapabilitiesOnly.value() << std::endl; }
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
        [[maybe_unused]] auto selectPreferredSurfaceFormat = [&physicalDevice, &surface]() -> bool {
            return true;
        };

        // user defined so that the user can choose specifically which present mode to use
        [[maybe_unused]] auto selectPreferredPresentMode = [&physicalDevice, &surface]() -> bool {
            return true;
        };

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

        // vertex buffer creation
        std::vector<Vertex> vertices = {
            Vertex { Math::Vector2(-0.5f, -0.5f), Math::Vector3(1.0f, 0.0f, 0.0f) },
            Vertex { Math::Vector2( 0.5f, -0.5f), Math::Vector3(0.0f, 1.0f, 0.0f) },
            Vertex { Math::Vector2( 0.5f,  0.5f), Math::Vector3(0.0f, 0.0f, 1.0f) },
            Vertex { Math::Vector2(-0.5f,  0.5f), Math::Vector3(1.0f, 1.0f, 1.0f) }
        };

        std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        std::vector<VkVertexInputBindingDescription> vertexInputBinding0Description = { GenerateVertexInputBindingDescription(0) };
        std::vector<VkVertexInputAttributeDescription> vertexInputBinding0Attributes = {
            GenerateVertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position)),
            GenerateVertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
        };

        auto&& [vertexBuffer, vertexBufferMemory] = CreateVertexBuffer(
            vertices,
            *physicalDevice,
            device,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            queueFamilyIndexWithGraphicsCapabilities.value(),
            graphicsQueue
        );

        auto&& [indexBuffer, indexBufferMemory] = CreateIndexBuffer(
            indices,
            *physicalDevice,
            device,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            queueFamilyIndexWithGraphicsCapabilities.value(),
            graphicsQueue
        );

        size_t uniformBufferSize = sizeof(UniformBufferObject);

        std::vector<Buffer> uniformBuffers {};
        std::vector<DeviceMemory> uniformBuffersMemory {};
        std::vector<void*> uniformBuffersMapped {};

        uniformBuffers.reserve(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.reserve(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.reserve(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            auto [uniformBuffer, uniformBufferMemory] = CreateBuffer(
                *physicalDevice,
                device,
                uniformBufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_SHARING_MODE_EXCLUSIVE,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );

            uniformBuffers.emplace_back(std::move(uniformBuffer));
            uniformBuffersMemory.emplace_back(std::move(uniformBufferMemory));
            uniformBuffersMapped.emplace_back(nullptr);

            auto uniformBufferMemoryMapInfo = GenerateMemoryMapInfo(uniformBuffersMemory[i], uniformBufferSize, 0);
            uniformBuffersMemory[i].Map(uniformBufferMemoryMapInfo, &uniformBuffersMapped[i]);
        }

        std::vector<VkDescriptorSetLayoutBinding> uniformBufferDescriptorSetLayoutBinding = { 
            GenerateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT)
        };
        auto uniformBufferDescriptorSetCreateInfo = GenerateDescriptorSetLayoutCreateInfo(uniformBufferDescriptorSetLayoutBinding);
        std::vector<DescriptorSetLayout> descriptorSetLayouts {};
        descriptorSetLayouts.emplace_back(uniformBufferDescriptorSetCreateInfo, device);

        std::vector<VkDescriptorPoolSize> descriptorPoolSizes = { 
            GenerateDescriptorPoolSize(
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
            )
        };
        auto descriptorPoolCreateInfo = GenerateDescriptorPoolCreateInfo(
            descriptorPoolSizes,
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
        );
        auto descriptorPool = DescriptorPool(descriptorPoolCreateInfo, device);

        std::vector<VkDescriptorSetLayout> layouts {};
        layouts.reserve(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            layouts.push_back(descriptorSetLayouts[0].Handle());
        }

        auto descriptorSetAllocateInfo = GenerateDescriptorSetAllocateInfo(
            descriptorPool,
            layouts
        );

        auto descriptorSets = DescriptorSets(descriptorSetAllocateInfo, device, descriptorPool);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            auto descriptorBufferInfo = GenerateDescriptorBufferInfo(uniformBuffers[i], sizeof(UniformBufferObject));

            auto writeBufferDescriptorSet = GenerateWriteDescriptorSet(
                1,
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                descriptorSets.Handles()[i],
                0,
                0,
                &descriptorBufferInfo
            );

            vkUpdateDescriptorSets(device.Handle(), 1, &writeBufferDescriptorSet, 0, nullptr);
        }

        auto pipelineVertexInputStateCreateInfo = GeneratePipelineVertexInputStateCreateInfo(vertexInputBinding0Description, vertexInputBinding0Attributes); // specify pipeline vertex inputs
        auto pipelineInputAssemblyStateCreateInfo = GeneratePipelineInputAssemblyStateCreateInfo(); // specify pipeline input assemblies
        
        std::vector<VkViewport> viewport = { GenerateViewport(static_cast<float>(swapchainExtent.width), static_cast<float>(swapchainExtent.height)) }; // specify viewport
        std::vector<VkRect2D> scissor = { GenerateRect2D(swapchainExtent) }; // specify scissor
        auto pipelineViewportStateCreateInfo = GeneratePipelineViewportStateCreateInfo(viewport, scissor);

        auto pipelineRasterizationStateCreateInfo = GeneratePipelineRasterizationStateCreateInfo(); // specify pipeline rasterization state
        auto pipelineMultisampleStateCreateInfo = GeneratePipelineMultisampleStateCreateInfo(); // specify pipeline multisampling state
        //auto pipelineDepthStencilStateCreateInfo = GeneratePipelineDepthStencilStateCreateInfo(); // specify pipeline depth stencil state

        std::vector<VkPipelineColorBlendAttachmentState> pipelineColorBlendAttachmentState =  { GeneratePipelineColorBlendAttachmentState() }; // specify pipeline color blend attachment state
        auto pipelineColorBlendStateCreateInfo = GeneratePipelineColorBlendStateCreateInfo(pipelineColorBlendAttachmentState);

        std::vector<VkDescriptorSetLayout> descriptorSetLayoutsHandle {};
        descriptorSetLayoutsHandle.reserve(descriptorSetLayouts.size());
        for (auto const& layout : descriptorSetLayouts) {
            descriptorSetLayoutsHandle.emplace_back(layout.Handle());
        }

        auto pipelineLayoutCreateInfo = GeneratePipelineLayoutCreateInfo(descriptorSetLayoutsHandle, {}); // create pipeline layout
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
        
        /// TODO: MAKE THIS SHIT WORK. NEED A WHOLE LOT OF REFACTOR
        //// TO DELETE //////////////////////////////////////////////////////////////////////////////////////////////

        // auto commandBuffersAllocateInfoTmp = commandBufferAllocateInfo;
        // commandBuffersAllocateInfoTmp.commandBufferCount = 5;

        // std::cout << "Allocating " << commandBuffersAllocateInfoTmp.commandBufferCount << " command buffers" << std::endl;

        // auto commandBuffersTmp = CommandBuffers(commandBuffersAllocateInfoTmp, device, commandPool);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

        while (running) {
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

            UpdateUniformBuffer(uniformBuffersMapped[frameIndex], static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT);

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

            std::vector<Buffer*> vertexBuffers = { &vertexBuffer };
            std::vector<VkDeviceSize> offsets = { 0 };
            std::vector<VkDeviceSize> sizes = { sizeof(vertices[0]) * vertices.size() };
            commandBuffer.BindVertexBuffers(0, vertexBuffers, offsets, sizes, {});
            
            commandBuffer.BindIndexBuffers(
                indexBuffer,
                0,
                (sizeof(indices[0]) * indices.size()),
                VK_INDEX_TYPE_UINT16
            );
            std::vector<VkDescriptorSet> descriptorSetsToBind = { descriptorSets.Handles()[frameIndex] };
            auto bindDescriptorSetsInfo = GenerateBindDescriptorSetsInfo(
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT,
                0,
                descriptorSetsToBind,
                {}
            );
            commandBuffer.BindDescriptorSets(bindDescriptorSetsInfo);

            commandBuffer.DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

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
        return CleanOnExit(EXIT_FAILURE);
    }

    return CleanOnExit(EXIT_SUCCESS);
}
