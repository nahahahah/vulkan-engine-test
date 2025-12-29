#include "Application.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

std::vector<Vertex> shapeVertices = {
    Vertex { Math::Vector3(-0.5f, -0.5f, 0.0f), Math::Vector3(1.0f, 0.0f, 0.0f), Math::Vector2(1.0f, 0.0f) },
    Vertex { Math::Vector3( 0.5f, -0.5f, 0.0f), Math::Vector3(0.0f, 1.0f, 0.0f), Math::Vector2(0.0f, 0.0f) },
    Vertex { Math::Vector3( 0.5f,  0.5f, 0.0f), Math::Vector3(0.0f, 0.0f, 1.0f), Math::Vector2(0.0f, 1.0f) },
    Vertex { Math::Vector3(-0.5f,  0.5f, 0.0f), Math::Vector3(1.0f, 1.0f, 1.0f), Math::Vector2(1.0f, 1.0f) },

    Vertex { Math::Vector3(-0.5f, -0.5f, -0.5f), Math::Vector3(1.0f, 0.0f, 0.0f), Math::Vector2(0.0f, 0.0f) },
    Vertex { Math::Vector3( 0.5f, -0.5f, -0.5f), Math::Vector3(0.0f, 1.0f, 0.0f), Math::Vector2(1.0f, 0.0f) },
    Vertex { Math::Vector3( 0.5f,  0.5f, -0.5f), Math::Vector3(0.0f, 0.0f, 1.0f), Math::Vector2(1.0f, 1.0f) },
    Vertex { Math::Vector3(-0.5f,  0.5f, -0.5f), Math::Vector3(1.0f, 1.0f, 1.0f), Math::Vector2(0.0f, 1.0f) }
};

std::vector<uint16_t> shapeIndices = {
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4
};

Application::Application() {
    std::clog << "Vulkan header version: " << VK_HEADER_VERSION << std::endl;

    try {
        InitSDL();
        InitWindow();
        InitVulkan();
    }

    catch (std::exception const& e) {
        throw e;
    }

    _running = true;
}

Application::~Application() {
    QuitSDL();
}

void Application::Run() {
    try {
        MainLoop(); /// TODO: Remove this extra call (potentially remove MainLoop as it's redundant)
        _device->WaitIdle();
    }
    
    catch (std::exception const& e) {
        throw e;
    }
}

// debug callback
#ifndef NDEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL Application::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* pUserData
) {
    (void)(pUserData);

    std::clog << "\n[ ";
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

    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl << std::endl;
    return VK_FALSE;
}
#endif

void Application::InitSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::string error = "Unable to initialize the SDL (reason: " + std::string(SDL_GetError()) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "SDL initialized successfully" << std::endl;
}

void Application::QuitSDL() {
    SDL_Quit();
}

void Application::InitWindow() {
    try {
        _window = std::make_unique<Window>(1200, 1000);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::InitVulkan() {
    CreateInstance();
#ifndef NDEBUG
    SetupDebugMessenger();
#endif
    CreateSurface();
    SelectPhysicalDevice();
    CreateDevice();
    CreateQueues();
    CreateSwapchain();
    GetSwapchainImages();
    CreateImageViews();
    CreateRenderPass();
    CreateDescriptorSetLayout();
    CreateGraphicsPipeline();
    CreateCommandPool();
    CreateDepthResources();
    CreateFramebuffers();
    CreateTextureImage();
    CreateTextureImageView();
    CreateTextureSampler();
    LoadModel();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();
    CreateCommandBuffers();
    CreateSynchronizationObjects();
}

void Application::CreateInstance() {
    auto applicationInfo = GenerateApplicationInfo(); // create application info
        auto instanceLayersProperties = EnumerateLayerProperties(); // enumerate instance layers properties

        bool validationLayersSupported = AreValidationLayerSupported(validationLayers, instanceLayersProperties); // check validation layers support and enable them if available
        if (enableValidationLayers && !validationLayersSupported) {
            std::string error = "Validation layers requested, but not available";
            throw std::runtime_error(error);
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

        auto instanceExtensionsProperties = EnumerateInstanceExtensionProperties();

    #ifdef NDEBUG
        //auto instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
        auto instanceCreateInfo = GenerateInstanceCreateInfo(&applicationInfo, enabledExtensions, validationLayers);

        try {
            _instance = std::make_unique<Instance>(instanceCreateInfo); // create instance
        }

        catch (std::exception const& e) {
            throw e;
        }
    
    #else
        auto instanceCreateInfo = GenerateInstanceCreateInfo(
            &applicationInfo,
            enabledExtensions,
            validationLayers,
            0 /* VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR */
        );

        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo {};
        if (enableValidationLayers) {
            debugUtilsMessengerCreateInfo = GenerateDebugUtilsMessengerCreateInfo(DebugCallback);
            instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;
        }
        
        try {
            _instance = std::make_unique<Instance>(instanceCreateInfo); // create instance
        }

        catch (std::exception const& e) {
            throw e;
        }

    #endif
}

#ifndef NDEBUG
void Application::SetupDebugMessenger() {
    if (!enableValidationLayers) {
        return;
    }

    try {
        auto debugUtilsMessengerCreateInfo = GenerateDebugUtilsMessengerCreateInfo(DebugCallback);
        _debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(debugUtilsMessengerCreateInfo, *_instance); // create the global debug messenger
    }

    catch (std::exception const& e) {
        throw e;
    }
}
#endif

void Application::CreateSurface() {
    try {
        _surface = std::make_unique<Surface>(*_instance, *_window); // create the surface from SDL
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::SelectPhysicalDevice() {
    auto physicalDevices = PhysicalDeviceCollection(*_instance); // enumerate physical devices

    if (physicalDevices.size() == 0) {
        std::string error = "Could not find GPUs with Vulkan support";
        throw std::runtime_error(error);
    }

    // check if device is suitable
    for (auto& physicalDevice : physicalDevices) {
        if (IsPhysicalDeviceSuitable(physicalDevice)) {
            _physicalDevice = std::make_unique<PhysicalDevice>(std::move(physicalDevice));
            break;
        }
    }

    if (_physicalDevice == nullptr) {
        std::string error = "Could not find a suitable GPU";
        throw std::runtime_error(error);
    }
}

void Application::CreateDevice() {    
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(*_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {};
    std::set<uint32_t> uniqueQueueFamilies = { 
        queueFamilyIndices.graphicsFamily.value(), 
        queueFamilyIndices.presentFamily.value()
    };
    queueCreateInfos.reserve(uniqueQueueFamilies.size());

    float queuePriority = 1.0f;
    for (auto queueFamily : uniqueQueueFamilies) {
        queueCreateInfos.emplace_back(GenerateDeviceQueueCreateInfo(queueFamily, &queuePriority));
    }

    auto physicalDeviceFeatures = _physicalDevice->Features();
    auto physicalDeviceSynchronizationFeature2 = GeneratePhysicalDeviceSynchronization2Features(); // enable synchronization2 feature

    auto deviceCreateInfo = GenerateDeviceCreateInfo(
        queueCreateInfos,
        deviceExtensions,
        {},
        physicalDeviceFeatures,
        0,
        &physicalDeviceSynchronizationFeature2
    );

    if (enableValidationLayers) {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }

    try {
        _device = std::make_unique<Device>(deviceCreateInfo, *_physicalDevice);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateQueues() {
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(*_physicalDevice);

    try {
        auto graphicsQueueInfo = GenerateDeviceQueueInfo(0, queueFamilyIndices.graphicsFamily.value());
        _graphicsQueue = std::make_unique<Queue>(graphicsQueueInfo, *_device);

        auto presentQueueInfo = GenerateDeviceQueueInfo(0, queueFamilyIndices.presentFamily.value());
        _presentQueue = std::make_unique<Queue>(presentQueueInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateSwapchain() {
    SwapchainSupportDetails swapchainSupportDetails = QuerySwapchainSupport(*_physicalDevice);

    auto surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupportDetails.formats);
    auto presentMode = ChooseSwapPresentMode(swapchainSupportDetails.presentModes);
    auto extent = ChooseSwapExtent(swapchainSupportDetails.capabilities);

    uint32_t imageCount = swapchainSupportDetails.capabilities.surfaceCapabilities.minImageCount + 1;
    if (swapchainSupportDetails.capabilities.surfaceCapabilities.maxImageCount > 0
     && imageCount > swapchainSupportDetails.capabilities.surfaceCapabilities.maxImageCount) {
        imageCount = swapchainSupportDetails.capabilities.surfaceCapabilities.maxImageCount;
    }

    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(*_physicalDevice);

    auto swapchainCreateInfo = GenerateSwapchainCreateInfo(
        surfaceFormat.surfaceFormat.colorSpace,
        extent,
        surfaceFormat.surfaceFormat.format,
        queueFamilyIndices.graphicsFamily.value(),
        queueFamilyIndices.presentFamily.value(),
        imageCount,
        presentMode,
        swapchainSupportDetails.capabilities.surfaceCapabilities.currentTransform,
        *_surface
    );

    try {
        _swapchain = std::make_unique<Swapchain>(swapchainCreateInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }

    _swapchainImageFormat = surfaceFormat.surfaceFormat.format;
    _swapchainExtent = extent;
}

void Application::GetSwapchainImages() {
    try {
        _swapchainImages = _device->SwapchainImages(*_device, *_swapchain);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateImageViews() {
    try {
        _swapchainImageViews.reserve(_swapchainImages.size());
        for (size_t i = 0; i < _swapchainImages.size(); ++i) {
            _swapchainImageViews.emplace_back(CreateImageView(_swapchainImages[i], _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT)); // create swap chain image view
        }
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateRenderPass() {
    std::vector<VkAttachmentDescription2> attachmentsDescription {}; // specify attachments description
    attachmentsDescription.reserve(2);
    
    auto colorAttachmentDescription = GenerateAttachmentDescription(_swapchainImageFormat);
    auto depthAttachmentDescription = GenerateAttachmentDescription(FindDepthFormat());
    depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachmentsDescription.emplace_back(colorAttachmentDescription);
    attachmentsDescription.emplace_back(depthAttachmentDescription);

    std::vector<VkAttachmentReference2> colorAttachmentsReference {}; // create color attachments reference
    colorAttachmentsReference.reserve(1);

    auto colorAttachmentReference = GenerateAttachmentReference(0);
    colorAttachmentsReference.emplace_back(colorAttachmentReference);

    auto depthAttachmentReference = GenerateAttachmentReference(1); // create depth attachment reference
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::vector<VkSubpassDescription2> subpassDescription = { GenerateSubpassDescription(colorAttachmentsReference, &depthAttachmentReference) }; // specify subpass descriptions
    
    std::vector<VkSubpassDependency2> subpassesDependency = { GenerateSubpassDependency() }; // specify subpass dependencies
    subpassesDependency[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    subpassesDependency[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    subpassesDependency[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    subpassesDependency[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    auto renderPassCreateInfo = GenerateRenderPassCreateInfo(attachmentsDescription, subpassesDependency, subpassDescription);
    
    try {
        _renderPass = std::make_unique<RenderPass>(renderPassCreateInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateDescriptorSetLayout() {
    std::vector<VkDescriptorSetLayoutBinding> uniformBufferDescriptorSetLayoutBindings = { 
        GenerateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT),
        GenerateDescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT),
    };
    auto uniformBufferDescriptorSetCreateInfo = GenerateDescriptorSetLayoutCreateInfo(uniformBufferDescriptorSetLayoutBindings);
    
    try {
        _descriptorSetLayout = std::make_unique<DescriptorSetLayout>(uniformBufferDescriptorSetCreateInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateGraphicsPipeline() {
    std::vector<char> vertexShaderFileBuffer;
    auto vertexShaderModuleCreateInfo = GenerateShaderModuleCreateInfo("resources/shaders/triangle.vertex.spv", vertexShaderFileBuffer);
    auto vertexShaderModule = ShaderModule(vertexShaderModuleCreateInfo, *_device); // create vertex shader module

    std::vector<char> fragmentShaderFileBuffer;
    auto fragmentShaderModuleCreateInfo = GenerateShaderModuleCreateInfo("resources/shaders/triangle.fragment.spv", fragmentShaderFileBuffer);
    auto fragmentShaderModule = ShaderModule(fragmentShaderModuleCreateInfo, *_device); // create fragment shader module
    
    std::string shaderMainFunctionName = "main";
    auto vertexShaderPipelineStageCreateInfo = GeneratePipelineShaderStageCreateInfo(vertexShaderModule, shaderMainFunctionName, VK_SHADER_STAGE_VERTEX_BIT); // create vertex shader pipeline stage
    auto fragmentShaderPipelineStageCreateInfo = GeneratePipelineShaderStageCreateInfo(fragmentShaderModule, shaderMainFunctionName, VK_SHADER_STAGE_FRAGMENT_BIT); // create fragment shader pipeline stage

    std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos = { vertexShaderPipelineStageCreateInfo, fragmentShaderPipelineStageCreateInfo };
    
    std::vector<VkVertexInputBindingDescription> vertexInputBinding0Description = { GenerateVertexInputBindingDescription(0) };
    std::vector<VkVertexInputAttributeDescription> vertexInputBinding0Attributes = {
        GenerateVertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)),
        GenerateVertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)),
        GenerateVertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv))
    };

    auto pipelineVertexInputStateCreateInfo = GeneratePipelineVertexInputStateCreateInfo(vertexInputBinding0Description, vertexInputBinding0Attributes); // specify pipeline vertex inputs
    auto pipelineInputAssemblyStateCreateInfo = GeneratePipelineInputAssemblyStateCreateInfo(); // specify pipeline input assemblies

    std::vector<VkViewport> viewport = { GenerateViewport(static_cast<float>(_swapchainExtent.width), static_cast<float>(_swapchainExtent.height)) }; // specify viewport
    std::vector<VkRect2D> scissor = { GenerateRect2D(_swapchainExtent) }; // specify scissor
    auto pipelineViewportStateCreateInfo = GeneratePipelineViewportStateCreateInfo(viewport, scissor);

    auto pipelineRasterizationStateCreateInfo = GeneratePipelineRasterizationStateCreateInfo(); // specify pipeline rasterization state
    pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
    pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    auto pipelineMultisampleStateCreateInfo = GeneratePipelineMultisampleStateCreateInfo(); // specify pipeline multisampling state
    auto pipelineDepthStencilStateCreateInfo = GeneratePipelineDepthStencilStateCreateInfo(); // specify pipeline depth stencil state

    std::vector<VkPipelineColorBlendAttachmentState> pipelineColorBlendAttachmentState =  { GeneratePipelineColorBlendAttachmentState() }; // specify pipeline color blend attachment state
    auto pipelineColorBlendStateCreateInfo = GeneratePipelineColorBlendStateCreateInfo(pipelineColorBlendAttachmentState);

    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    auto pipelineDynamicStateCreateInfo = GeneratePipelineDynamicStateCreateInfo(dynamicStates); // specify pipeline dynamic states        

    std::vector<VkDescriptorSetLayout> descriptorSetLayoutsHandles {};
    descriptorSetLayoutsHandles.reserve(1);
    descriptorSetLayoutsHandles.emplace_back(_descriptorSetLayout->Handle());
    
    try {
        auto pipelineLayoutCreateInfo = GeneratePipelineLayoutCreateInfo(descriptorSetLayoutsHandles, {}); // create pipeline layout
        
        _pipelineLayout = std::make_unique<PipelineLayout>(pipelineLayoutCreateInfo, *_device);
    
        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = GenerateGraphicsPipelineCreateInfo(
            pipelineDynamicStateCreateInfo,
            pipelineVertexInputStateCreateInfo,
            pipelineInputAssemblyStateCreateInfo,
            pipelineViewportStateCreateInfo,
            pipelineRasterizationStateCreateInfo,
            pipelineMultisampleStateCreateInfo,
            pipelineDepthStencilStateCreateInfo,
            pipelineColorBlendStateCreateInfo,
            *_pipelineLayout,
            pipelineShaderStageCreateInfos,
            *_renderPass
        );
        
        _graphicsPipeline = std::make_unique<Pipeline>(graphicsPipelineCreateInfo, *_device); // create graphics pipeline
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateFramebuffers() {
    try {
        _framebuffers.reserve(_swapchainImageViews.size());
        for (int i = 0; i < static_cast<int>(_swapchainImageViews.size()); ++i) {
            std::vector<VkImageView> attachments = {
                _swapchainImageViews[i].Handle(),
                _depthImageView->Handle()
            };
            auto frameBufferCreateInfo = GenerateFramebufferCreateInfo(_swapchainExtent, attachments, *_renderPass);
            
            _framebuffers.emplace_back(frameBufferCreateInfo, *_device); // create frame buffer
        }
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateCommandPool() {
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(*_physicalDevice);

    auto commandPoolCreateInfo = GenerateCommandPoolCreateInfo(queueFamilyIndices.graphicsFamily.value());
    
    try {
        _commandPool = std::make_unique<CommandPool>(commandPoolCreateInfo, *_device); // create command pool
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateDepthResources() {
    try {
        VkFormat depthFormat = FindDepthFormat();

        CreateImage(
            { _swapchainExtent.width, _swapchainExtent.height, 1 },
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            depthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            _depthImage,
            _depthImageMemory
        );
        _depthImageView = std::make_unique<ImageView>(CreateImageView(*_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT));

        TransitionImageLayout(
            _depthImage,
            depthFormat,
            VK_IMAGE_ASPECT_DEPTH_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        );
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateTextureImage() {
    int textureWidth = 0;
    int textureHeight = 0;
    int textureChannels = 0;
    stbi_uc* pixels = stbi_load(
        "resources/textures/crate.png",
        &textureWidth,
        &textureHeight,
        &textureChannels,
        STBI_rgb_alpha
    );

    if (!pixels) {
        std::string error = "Could not load the texture image file (reason: " + std::string(stbi_failure_reason()) + ")";
        throw std::runtime_error(error);
    }

    VkDeviceSize imageSize = textureWidth * textureHeight * static_cast<int>(STBI_rgb_alpha);

    std::unique_ptr<Buffer> stagingBuffer = nullptr;
    std::unique_ptr<DeviceMemory> stagingBufferMemory = nullptr;
    CreateBuffer(
        imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void* data = nullptr;
    auto stagingBufferMemoryMapInfo = GenerateMemoryMapInfo(*stagingBufferMemory, imageSize, 0);
    _device->MapMemory(stagingBufferMemoryMapInfo, &data);

    std::memcpy(data, pixels, static_cast<size_t>(imageSize));
    
    auto stagingBufferMemoryUnmapInfo = GenerateMemoryUnmapInfo(*stagingBufferMemory);
    _device->UnmapMemory(stagingBufferMemoryUnmapInfo);

    stbi_image_free(pixels);

    CreateImage(
        VkExtent3D { 
            static_cast<uint32_t>(textureWidth),
            static_cast<uint32_t>(textureHeight),
            1
        },
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        _textureImage,
        _textureImageMemory
    );

    TransitionImageLayout(
        _textureImage,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    CopyBufferToImage(
        *stagingBuffer,
        *_textureImage,
        static_cast<uint32_t>(textureWidth),
        static_cast<uint32_t>(textureHeight)
    );

    TransitionImageLayout(
        _textureImage,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
}

void Application::CreateTextureImageView() {
    try {
        _textureImageView = std::make_unique<ImageView>(CreateImageView(*_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT));
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateTextureSampler() {
    try {
        auto samplerCreateInfo = GenerateSamplerCreateInfo();
        samplerCreateInfo.anisotropyEnable = VK_TRUE;
        samplerCreateInfo.maxAnisotropy = _physicalDevice->Properties().properties.limits.maxSamplerAnisotropy;
        _textureSampler = std::make_unique<Sampler>(samplerCreateInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::LoadModel() {
    tinyobj::attrib_t attrib {};
    std::vector<tinyobj::shape_t> shapes {};
    std::vector<tinyobj::material_t> materials {};
    std::string warnings = "";
    std::string errors = "";

    bool result = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warnings,
        &errors,
        "resources/models/crate.obj"
    );
    if (!result) {
        std::string error = "Could not load a model:\nWarnings:\n" + warnings + "\nErrors:\n" + errors; 
        throw std::runtime_error(error);
    }

    for (auto const& shape : shapes) {
        for (auto const& index : shape.mesh.indices) {
            Vertex vertex {};

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.uv = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            vertices.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }
    
}

void Application::CreateVertexBuffer() {
    try {
        VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();

        std::unique_ptr<Buffer> stagingBuffer = nullptr;
        std::unique_ptr<DeviceMemory> stagingBufferMemory = nullptr;
        CreateBuffer(
            vertexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void* data = nullptr;
        auto stagingBufferMemoryMapInfo = GenerateMemoryMapInfo(*stagingBufferMemory, vertexBufferSize, 0);
        _device->MapMemory(stagingBufferMemoryMapInfo, &data);
        
        std::memcpy(data, vertices.data(), static_cast<size_t>(vertexBufferSize));

        auto stagingBufferMemoryUnmapInfo = GenerateMemoryUnmapInfo(*stagingBufferMemory);
        _device->UnmapMemory(stagingBufferMemoryUnmapInfo);

        CreateBuffer(
            vertexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _vertexBuffer,
            _vertexBufferMemory
        );

        CopyBuffer(*stagingBuffer, *_vertexBuffer, vertexBufferSize);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateIndexBuffer() {
    try {
        VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

        std::unique_ptr<Buffer> stagingBuffer = nullptr;
        std::unique_ptr<DeviceMemory> stagingBufferMemory = nullptr;
        CreateBuffer(
            indexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void* data = nullptr;
        auto stagingBufferMemoryMapInfo = GenerateMemoryMapInfo(*stagingBufferMemory, indexBufferSize, 0);
        _device->MapMemory(stagingBufferMemoryMapInfo, &data);
        
        std::memcpy(data, indices.data(), static_cast<size_t>(indexBufferSize));

        auto stagingBufferMemoryUnmapInfo = GenerateMemoryUnmapInfo(*stagingBufferMemory);
        _device->UnmapMemory(stagingBufferMemoryUnmapInfo);

        CreateBuffer(
            indexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _indexBuffer,
            _indexBufferMemory
        );

        CopyBuffer(*stagingBuffer, *_indexBuffer, indexBufferSize);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateUniformBuffers() {
    size_t uniformBufferSize = sizeof(UniformBufferObject);

    _uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    _uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    _uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        CreateBuffer(
            uniformBufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            _uniformBuffers[i],
            _uniformBuffersMemory[i]
        );

        _uniformBuffersMapped[i] = nullptr;

        auto uniformBufferMemoryMapInfo = GenerateMemoryMapInfo(*_uniformBuffersMemory[i], uniformBufferSize, 0);
        _device->MapMemory(uniformBufferMemoryMapInfo, &_uniformBuffersMapped[i]);
    }
}

void Application::CreateDescriptorPool() {
    std::vector<VkDescriptorPoolSize> descriptorPoolSizes = { 
        GenerateDescriptorPoolSize(
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
        ),
        GenerateDescriptorPoolSize(
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
        )
    };

    auto descriptorPoolCreateInfo = GenerateDescriptorPoolCreateInfo(
        descriptorPoolSizes,
        static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
    );

    try {
        _descriptorPool = std::make_unique<DescriptorPool>(descriptorPoolCreateInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, _descriptorSetLayout->Handle());
    auto descriptorSetAllocateInfo = GenerateDescriptorSetAllocateInfo(
        *_descriptorPool,
        layouts
    );

    try {
        _descriptorSets = std::make_unique<DescriptorSetCollection>(descriptorSetAllocateInfo, *_device);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            auto descriptorBufferInfo = GenerateDescriptorBufferInfo(*_uniformBuffers[i], sizeof(UniformBufferObject));
            auto textureDescriptorImageInfo = GenerateDescriptorImageInfo(*_textureImageView, *_textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            std::vector<VkWriteDescriptorSet> bufferDescriptorSetsWrites = {
                GenerateWriteDescriptorSet(
                    1,
                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    (*_descriptorSets)[i],
                    0,
                    0,
                    &descriptorBufferInfo,
                    nullptr
                ),
                GenerateWriteDescriptorSet(
                    1,
                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    (*_descriptorSets)[i],
                    1,
                    0,
                    nullptr,
                    &textureDescriptorImageInfo
                )
            };

            _device->UpdateDescriptorSets(bufferDescriptorSetsWrites, {});
        }
    }
    
    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateCommandBuffers() {
    auto commandBufferAllocateInfo = GenerateCommandBufferAllocateInfo(*_commandPool, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)); // create command buffer
    
    try {
        _commandBuffers = std::make_unique<CommandBufferCollection>(commandBufferAllocateInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateSynchronizationObjects() {
    auto semaphoreCreateInfo = GenerateSemaphoreCreateInfo();
    auto fenceCreateInfo = GenerateFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT); // signaled on creation to avoid deadlock on the first frame

    _imageAvailableSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.reserve(_swapchainImages.size());
    _inFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);

    try {
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            _imageAvailableSemaphores.emplace_back(semaphoreCreateInfo, *_device);
            _inFlightFences.emplace_back(fenceCreateInfo, *_device);
        }

        for (int i = 0; i < static_cast<int>(_swapchainImages.size()); ++i) {
            _renderFinishedSemaphores.emplace_back(semaphoreCreateInfo, *_device);
        }
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::MainLoop() {
    while (_running) {
        Fence& frameFence = _inFlightFences[_frameIndex];
        std::vector<VkFence> fencesToResetAndWaitFor = { frameFence.Handle() };
        _device->WaitForFences(fencesToResetAndWaitFor);

        uint32_t imageIndex = 0;
        Semaphore& acquireSemaphore = _imageAvailableSemaphores[_frameIndex];
        CommandBuffer& commandBuffer = (*_commandBuffers)[_frameIndex];

        while (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_EVENT_QUIT) {
                _running = false;
            }

            if (_event.type == SDL_EVENT_WINDOW_RESIZED ) {
                _framebufferResized = true;
                std::clog << "Window resized to " << _event.window.data1 << "x" << _event.window.data2 << std::endl;
            }
        }

        auto acquireNextImageInfo = GenerateAcquireNextImageInfo(*_swapchain, &acquireSemaphore);
        VkResult acquireNextImageResult = _device->AcquireNextImage(acquireNextImageInfo, &imageIndex); // acquire next frame
        if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
            std::cerr << "Swap chain image is out of date" << std::endl;

            RecreateSwapchain();
            return;
        }

        else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Could not aquire swapchain image");
        }

        UpdateUniformBuffer(_frameIndex);

        _device->ResetFences(fencesToResetAndWaitFor);
        
        // REALLY IMPORTANT: USE SWAPCHAIN IMAGE INDEX RETRIEVED FROM ANI TO PROPERLY RE-USE SEMAPHORES
        Semaphore& submitSemaphore = _renderFinishedSemaphores[imageIndex];

        commandBuffer.Reset();
        RecordCommandBuffer((*_commandBuffers)[_frameIndex], imageIndex);

        std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfo = { GenerateCommandBufferSubmitInfo(commandBuffer) }; // specify command buffer submit info for submission
        std::vector<VkSemaphoreSubmitInfo> signalSemaphoreSubmitInfo = { GenerateSemaphoreSubmitInfo(submitSemaphore, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT) };
        std::vector<VkSemaphore> signalSemaphores = { submitSemaphore.Handle() };
        std::vector<VkSemaphoreSubmitInfo> waitSemaphoreSubmitInfo = { GenerateSemaphoreSubmitInfo(acquireSemaphore) }; // set synchronization handles behaviours for submission
        std::vector<VkSemaphore> waitSemaphores = { acquireSemaphore.Handle() };

        std::vector<VkSubmitInfo2> submitInfos = { GenerateSumbitInfo(commandBufferSubmitInfo, signalSemaphoreSubmitInfo, waitSemaphoreSubmitInfo) };
        _graphicsQueue->Submit(submitInfos, &frameFence); // submit command buffer to the queue

        std::vector<uint32_t> imageIndices = { imageIndex };
        std::vector<VkSwapchainKHR> swapchains = { _swapchain->Handle() };

        VkPresentInfoKHR presentInfo = GeneratePresentInfo(imageIndices, swapchains, signalSemaphores);
        VkResult queuePresentResult = _presentQueue->Present(presentInfo); // present
        if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR || _framebufferResized) {
            _framebufferResized = false;
            RecreateSwapchain();
        }
        
        else if (queuePresentResult != VK_SUCCESS) {
            std::string error = "Unable to present image with queue (status: " + std::to_string(queuePresentResult) + ")";
            throw std::runtime_error(error);
        }

        // cap frame index to MAX_FRAMES_IN_FLIGHT
        _frameIndex = (_frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}

void Application::RecordCommandBuffer(CommandBuffer& commandBuffer, uint32_t imageIndex) {
    auto commandBufferBeginInfo = GenerateCommandBufferBeginInfo();
    commandBuffer.Begin(commandBufferBeginInfo); // begin command buffer recording

    std::vector<VkClearValue> clearValues {};
    clearValues.reserve(2);
    clearValues.emplace_back(GenerateClearValue());
    clearValues.emplace_back(VkClearValue { .depthStencil = { 1.0f, 0 }});

    auto renderPassBeginInfoRenderArea = GenerateRect2D(_swapchainExtent);
    VkRenderPassBeginInfo renderPassBeginInfo = GenerateRenderPassBeginInfo(*_renderPass, _framebuffers[imageIndex], clearValues, renderPassBeginInfoRenderArea);
    auto subpassBeginInfo = GenerateSubpassBeginInfo();
    commandBuffer.BeginRenderPass(renderPassBeginInfo, subpassBeginInfo); // begin render pass

    commandBuffer.BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, *_graphicsPipeline);

    std::vector<VkViewport> viewports = { GenerateViewport(static_cast<float>(_swapchainExtent.width), static_cast<float>(_swapchainExtent.height)) };
    std::vector<VkRect2D> scissors = { GenerateRect2D(_swapchainExtent) };

    // set dynamic states
    commandBuffer.SetViewport(0, 1, viewports);
    commandBuffer.SetScissor(0, 1, scissors);

    std::vector<Buffer*> vertexBuffers = { _vertexBuffer.get() };
    std::vector<VkDeviceSize> offsets = { 0 };
    std::vector<VkDeviceSize> sizes = { sizeof(vertices[0]) * vertices.size() };
    commandBuffer.BindVertexBuffers(0, vertexBuffers, offsets, sizes, {});
    
    commandBuffer.BindIndexBuffers(
        *_indexBuffer,
        0,
        (sizeof(indices[0]) * indices.size()),
        VK_INDEX_TYPE_UINT32
    );
    
    std::vector<VkDescriptorSet> descriptorSetCollectionToBind = { (*_descriptorSets)[_frameIndex] };
    auto bindDescriptorSetCollectionInfo = GenerateBindDescriptorSetsInfo(
        *_pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        descriptorSetCollectionToBind,
        {}
    );
    commandBuffer.BindDescriptorSets(bindDescriptorSetCollectionInfo);

    commandBuffer.DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    auto subpassEndInfo = GenerateSubpassEndInfo();
    commandBuffer.EndRenderPass(subpassEndInfo); // end render pass

    commandBuffer.End();
}

CommandBuffer Application::BeginSingleTimeCommands() {
    try {
        auto allocateInfo = GenerateCommandBufferAllocateInfo(*_commandPool);
        auto commandBuffer = CommandBuffer(allocateInfo, *_device, *_commandPool);

        auto beginInfo = GenerateCommandBufferBeginInfo();
        commandBuffer.Begin(beginInfo);

        return std::move(commandBuffer);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::EndSingleTimeCommands(CommandBuffer& commandBuffer) {
    try {
        commandBuffer.End();

        std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfo = {
            GenerateCommandBufferSubmitInfo(commandBuffer)
        };
        std::vector<VkSubmitInfo2> submitInfos = { GenerateSumbitInfo(commandBufferSubmitInfo, {}, {}) };
        _graphicsQueue->Submit(submitInfos);
        _graphicsQueue->WaitIdle();
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::UpdateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    float ratio = _swapchainExtent.width / static_cast<float>(_swapchainExtent.height); 
    (void) ratio;

    UniformBufferObject ubo {};

    /*
    // if only a flat surface taking all the the window is needed
    ubo.model = glm::mat4(1.0);
    ubo.view = Math::Matrix4x4(1.0);
    ubo.projection = glm::mat4(1.0);
    */

    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.model *= glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    ubo.view = glm::lookAt(glm::vec3(15.0f, 12.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 10000.0f);
    ubo.projection[1][1] *= -1;

    int width = 0;
    int height = 0;
    if (!SDL_GetWindowSizeInPixels(_window->Handle(), &width, &height)) {
        std::string error = "Unable to get the window size in pixels: " + std::string(SDL_GetError());
        throw std::runtime_error(error);
    }

    ubo.resolution.x = static_cast<float>(width);
    ubo.resolution.y = static_cast<float>(height);


    ubo.time = time;

    std::memcpy(_uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Application::TransitionImageLayout(
    std::unique_ptr<Image>& image,
    VkFormat format,
    VkImageAspectFlags aspectMask,
    VkImageLayout oldLayout,
    VkImageLayout newLayout
) {
    (void)(format);

    CommandBuffer commandBuffer = BeginSingleTimeCommands();

    std::vector<VkImageMemoryBarrier2> barriers = { GenerateImageMemoryBarrier(*image, oldLayout, newLayout) };
    barriers[0].subresourceRange.aspectMask = aspectMask;
    auto dependencyInfo = GenerateDependencyInfo({}, {}, barriers);
    
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barriers[0].srcAccessMask = 0;
        barriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    
        barriers[0].srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
        barriers[0].dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    }

    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        barriers[0].srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
        barriers[0].dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    }

    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barriers[0].srcAccessMask = 0;
        barriers[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        barriers[0].srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
        barriers[0].dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;
    }

    else {
        throw std::invalid_argument("Unsuported layout transition");
    }

    commandBuffer.PipelineBarrier(dependencyInfo);

    EndSingleTimeCommands(commandBuffer);
}

QueueFamilyIndices Application::FindQueueFamilies(PhysicalDevice const& physicalDevice) {
    QueueFamilyIndices queueFamilyIndices {};

    auto properties = physicalDevice.QueueFamilyProperties(); // get physical device's queue family properties

    int queueFamilyIndex = 0;
    //std::clog << " - Queue families (count: " << physicalDeviceQueueFamilyProperties.size() << ", "  << physicalDeviceProperties.properties.deviceName << ")" << std::endl;
    for (auto const& queueFamilyProperties : properties) {
        if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.graphicsFamily = queueFamilyIndex; 
        }

        if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            queueFamilyIndices.transferFamily = queueFamilyIndex;
        }

        // check that the retrieved surface is supported by a specific queue family for the current physical device
        bool supportedSurface = physicalDevice.IsSurfaceSupportedByQueueFamily(*_surface, queueFamilyIndex);
        if (supportedSurface) {
            queueFamilyIndices.presentFamily = queueFamilyIndex;
        }

        if (queueFamilyIndices.IsComplete()) {
            break;
        }

        ++queueFamilyIndex;
    }

    return queueFamilyIndices;
}

SwapchainSupportDetails Application::QuerySwapchainSupport(PhysicalDevice const& physicalDevice) {
    SwapchainSupportDetails details {};
    
    auto physicalDeviceSurfaceInfo = GeneratePhysicalDeviceSurfaceInfo(*_surface); // get surface info
    details.capabilities = physicalDevice.SurfaceCapabilities(physicalDeviceSurfaceInfo); // get capabilities of the surface
    details.formats = physicalDevice.SurfaceFormats(physicalDeviceSurfaceInfo); // get formats of the surface
    details.presentModes = physicalDevice.PresentModes(*_surface); // get present modes of the surface

    return details;
}

bool Application::IsPhysicalDeviceSuitable(PhysicalDevice const& physicalDevice) {
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

    bool queueFamilyIndicesAreComplete = queueFamilyIndices.IsComplete();

    // enumerate device extensions
    auto deviceExtensionsProperties = EnumerateDeviceExtensionProperties(physicalDevice); // enumerate device extensions
    bool deviceExtensionsAreSupported = AreDeviceExtensionsSupported(deviceExtensions, deviceExtensionsProperties);
    //std::clog << (deviceExtensionsSupported ? "All device extensions are supported" : "Some or all device extensions aren't supported") << std::endl;

    // check is swapchain has mandatory properties
    bool swapchainIsAdequate = false;
    if (deviceExtensionsAreSupported) {
        SwapchainSupportDetails swapchainSupportDetails = QuerySwapchainSupport(physicalDevice);
        swapchainIsAdequate = !swapchainSupportDetails.formats.empty() && ! swapchainSupportDetails.presentModes.empty();
    }

    auto supportedFeatures = physicalDevice.Features();
    
    return queueFamilyIndicesAreComplete
        && deviceExtensionsAreSupported
        && swapchainIsAdequate
        && supportedFeatures.features.samplerAnisotropy;
}

uint32_t Application::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    auto memoryProperties = _physicalDevice->MemoryProperties().memoryProperties;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type");
}

VkFormat Application::FindSupportedFormat(std::vector<VkFormat> const& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const {
    for (auto format : candidates) {
        auto formatProperties = _physicalDevice->FormatProperties(format);

        if (tiling == VK_IMAGE_TILING_LINEAR
        && (formatProperties.formatProperties.linearTilingFeatures & features) == features) {
            return format;
        } 

           else if (tiling == VK_IMAGE_TILING_OPTIMAL
                && (formatProperties.formatProperties.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("Failed to find a supported format");
}

VkFormat Application::FindDepthFormat() const {
    try {
        return FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    catch (std::exception const& e) {
        throw e;
    }
}

bool Application::HasStencilComponent(VkFormat format) const {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkSurfaceFormat2KHR Application::ChooseSwapSurfaceFormat(std::span<VkSurfaceFormat2KHR> availableFormats) {
    for (auto const& availableFormat : availableFormats) {
        if (availableFormat.surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB
         && availableFormat.surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Application::ChooseSwapPresentMode(std::span<VkPresentModeKHR> availablePresentModes) {
    for (auto const availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Application::ChooseSwapExtent(VkSurfaceCapabilities2KHR const& surfaceCapabilities) {
    if (surfaceCapabilities.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return surfaceCapabilities.surfaceCapabilities.currentExtent;
    }
    
    else {
        return Swapchain::Extent2DFromSDLWindow(*_window, surfaceCapabilities);
    }
}

void Application::CreateBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties,
    std::unique_ptr<Buffer>& buffer,
    std::unique_ptr<DeviceMemory>& bufferMemory
) {
    try {
        auto bufferCreateInfo = GenerateBufferCreateInfo(size, usage, sharingMode);
        buffer = std::make_unique<Buffer>(bufferCreateInfo, *_device); // will be moved

        auto bufferMemoryRequirementsInfo = GenerateBufferMemoryRequirementsInfo(*buffer);
        auto bufferMemoryRequirements = _device->BufferMemoryRequirements(bufferMemoryRequirementsInfo);

        auto bufferMemoryAllocateInfo = GenerateMemoryAllocateInfo(
            bufferMemoryRequirements.memoryRequirements.size,
            FindMemoryType(
                bufferMemoryRequirements.memoryRequirements.memoryTypeBits,
                properties
            )
        );
        bufferMemory = std::make_unique<DeviceMemory>(bufferMemoryAllocateInfo, *_device); // will be moved

        std::vector<VkBindBufferMemoryInfo> bufferBindMemoryInfos = { GenerateBindBufferMemoryInfo(*buffer, *bufferMemory, 0) };
        _device->BindBufferMemory(bufferBindMemoryInfos);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateImage(
    VkExtent3D const& dimensions,
    VkImageUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties,
    VkFormat format,
    VkImageTiling tiling,
    std::unique_ptr<Image>& image,
    std::unique_ptr<DeviceMemory>& imageMemory
) {
    try {
        auto imageCreateInfo = GenerateImageCreateInfo(
            dimensions,
            usage,
            sharingMode,
            format,
            tiling
        );
        image = std::make_unique<Image>(imageCreateInfo, *_device);

        auto imageMemoryRequirementsInfo = GenerateImageMemoryRequirementsInfo(*image);
        auto imageMemoryRequirements = _device->ImageMemoryRequirements(imageMemoryRequirementsInfo);

        auto imageMemoryAllocateInfo = GenerateMemoryAllocateInfo(
            imageMemoryRequirements.memoryRequirements.size,
            FindMemoryType(
                imageMemoryRequirements.memoryRequirements.memoryTypeBits,
                properties
            )
        );

        imageMemory = std::make_unique<DeviceMemory>(imageMemoryAllocateInfo, *_device);

        std::vector<VkBindImageMemoryInfo> imageBindMemoryInfos = { GenerateBindImageMemoryInfo(*image, *imageMemory, 0) };
        _device->BindImageMemory(imageBindMemoryInfos);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

ImageView Application::CreateImageView(Image const& image, VkFormat format, VkImageAspectFlags aspectFlags) {
    try {
        auto imageViewCreateInfo = GenerateImageViewCreateInfo(format, image);
        imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;
        auto imageView = ImageView(imageViewCreateInfo, *_device);

        return std::move(imageView);
    }

    catch (std::exception const& e) {
        throw e;
    }
}


void Application::CopyBuffer(Buffer& src, Buffer& dst, VkDeviceSize size) {
    try {
        auto commandBuffer = BeginSingleTimeCommands();

        std::vector<VkBufferCopy2> bufferCopyRegions = { GenerateBufferCopy(size) };
        auto copyBufferInfo = GenerateCopyBufferInfo(src, dst, bufferCopyRegions);
        commandBuffer.CopyBuffer(copyBufferInfo);

        EndSingleTimeCommands(commandBuffer);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CopyBufferToImage(
    Buffer const& buffer,
    Image const& image,
    uint32_t width,
    uint32_t height
) {
    CommandBuffer commandBuffer = BeginSingleTimeCommands();

    std::vector<VkBufferImageCopy2> regions = { GenerateBufferImageCopy(width, height) };
    auto copyBufferToImageInfo = GenerateCopyBufferToImageInfo(
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        regions
    );
    commandBuffer.CopyBufferToImage(copyBufferToImageInfo);

    EndSingleTimeCommands(commandBuffer);
}

void Application::CleanupSwapchain() {
    _depthImageView.reset();
    _depthImage.reset();
    _depthImageMemory.reset();
    _framebuffers.clear();
    _swapchainImageViews.clear();
    _swapchainImages.clear();
    _swapchain.reset();
}

void Application::RecreateSwapchain() {
    int width = 0, height = 0;
    
    if (!SDL_GetWindowSizeInPixels(_window->Handle(), &width, &height)) {
        std::string error = "Could not get window size in pixels (reason: " + std::string(SDL_GetError()) + ")";
        throw std::runtime_error(error);
    }

    std::cout << "SDL_GetWindowFlags(_window->Handle()) & SDL_WINDOW_MINIMIZED = " << (SDL_GetWindowFlags(_window->Handle()) & SDL_WINDOW_MINIMIZED) << std::endl;
    
    while (SDL_GetWindowFlags(_window->Handle()) & SDL_WINDOW_MINIMIZED) {
        if (!SDL_GetWindowSizeInPixels(_window->Handle(), &width, &height)) {
            std::string error = "Could not get window size in pixels (reason: " + std::string(SDL_GetError()) + ")";
            throw std::runtime_error(error);
        }

        SDL_WaitEvent(&_event);
    }

    _device->WaitIdle();

    CleanupSwapchain();

    CreateSwapchain();
    GetSwapchainImages();
    CreateImageViews();
    CreateDepthResources();
    CreateFramebuffers();
}
