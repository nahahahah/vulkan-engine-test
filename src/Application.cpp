#include "Application.hpp"

Application::Application() {
    std::clog << "Vulkan header version: " << VK_HEADER_VERSION << std::endl;

    InitSDL();
    InitWindow();
    InitVulkan();

    _running = true;
}

Application::~Application() {
    QuitSDL();
}

void Application::Run() {
    MainLoop(); /// TODO: Remove this extra call (potentially remove MainLoop as it's redundant)

    try {
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
    SetupDebugMessenger();
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
    CreateFramebuffers();
    CreateCommandPool();
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

void Application::SetupDebugMessenger() {
    if (!enableValidationLayers) {
        return;
    }

    try {
        auto debugUtilsMessengerCreateInfo = GenerateDebugUtilsMessengerCreateInfo(DebugCallback);
        _debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(*_instance, debugUtilsMessengerCreateInfo); // create the global debug messenger
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateSurface() {
    try {
        _surface = std::make_unique<Surface>(*_instance, *_window); // create the surface from SDL
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::SelectPhysicalDevice() {
    auto physicalDevices = EnumeratedPhysicalDevices(*_instance); // enumerate physical devices

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
        _swapchainImages = EnumerateSwapChainImages(*_device, *_swapchain);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateImageViews() {
    try {
        _swapchainImageViews.reserve(_swapchainImages.size());
        for (size_t i = 0; i < _swapchainImages.size(); ++i) {
            auto swapchainImageViewCreateInfo = GenerateImageViewCreateInfo(_swapchainImageFormat, _swapchainImages[i]);
            _swapchainImageViews.emplace_back(swapchainImageViewCreateInfo, *_device); // create swap chain image view
        }
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateRenderPass() {
    std::vector<VkAttachmentDescription2> attachmentDescription = { GenerateAttachmentDescription(_swapchainImageFormat) }; // specify attachment description
    std::vector<VkAttachmentReference2> attachmentReference = { GenerateAttachmentReference() }; // create attachment reference
    std::vector<VkSubpassDescription2> subpassDescription = { GenerateSubpassDescription(attachmentReference) }; // specify subpass descriptions
    std::vector<VkSubpassDependency2> subpassDependency = { GenerateSubpassDependency() }; // specify subpass dependencies
    auto renderPassCreateInfo = GenerateRenderPassCreateInfo(attachmentDescription, subpassDependency, subpassDescription);
    
    try {
        _renderPass = std::make_unique<RenderPass>(renderPassCreateInfo, *_device);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateDescriptorSetLayout() {
    std::vector<VkDescriptorSetLayoutBinding> uniformBufferDescriptorSetLayoutBindings = { 
        GenerateDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT)
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
        GenerateVertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position)),
        GenerateVertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
    };

    auto pipelineVertexInputStateCreateInfo = GeneratePipelineVertexInputStateCreateInfo(vertexInputBinding0Description, vertexInputBinding0Attributes); // specify pipeline vertex inputs
    auto pipelineInputAssemblyStateCreateInfo = GeneratePipelineInputAssemblyStateCreateInfo(); // specify pipeline input assemblies

    std::vector<VkViewport> viewport = { GenerateViewport(static_cast<float>(_swapchainExtent.width), static_cast<float>(_swapchainExtent.height)) }; // specify viewport
    std::vector<VkRect2D> scissor = { GenerateRect2D(_swapchainExtent) }; // specify scissor
    auto pipelineViewportStateCreateInfo = GeneratePipelineViewportStateCreateInfo(viewport, scissor);

    auto pipelineRasterizationStateCreateInfo = GeneratePipelineRasterizationStateCreateInfo(); // specify pipeline rasterization state
    pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE /* VK_CULL_MODE_BACK_BIT */;
    pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    auto pipelineMultisampleStateCreateInfo = GeneratePipelineMultisampleStateCreateInfo(); // specify pipeline multisampling state
    //auto pipelineDepthStencilStateCreateInfo = GeneratePipelineDepthStencilStateCreateInfo(); // specify pipeline depth stencil state

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
            std::vector<VkImageView> attachments = { _swapchainImageViews[i].Handle() };
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

void Application::CreateVertexBuffer() {
    try {
        VkDeviceSize vertexBufferSize = sizeof(shapeVertices[0]) * shapeVertices.size();

        auto stagingBuffer = Buffer();
        auto stagingBufferMemory = DeviceMemory();
        CreateBuffer(
            vertexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void* data = nullptr;
        auto stagingBufferMemoryMapInfo = GenerateMemoryMapInfo(stagingBufferMemory, vertexBufferSize, 0);
        stagingBufferMemory.Map(stagingBufferMemoryMapInfo, &data);
        
        std::memcpy(data, shapeVertices.data(), static_cast<size_t>(vertexBufferSize));

        auto stagingBufferMemoryUnmapInfo = GenerateMemoryUnmapInfo(stagingBufferMemory);
        stagingBufferMemory.Unmap(stagingBufferMemoryUnmapInfo);

        _vertexBuffer = std::make_unique<Buffer>();
        _vertexBufferMemory = std::make_unique<DeviceMemory>();
        CreateBuffer(
            vertexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            *_vertexBuffer,
            *_vertexBufferMemory
        );

        CopyBuffer(stagingBuffer, *_vertexBuffer, vertexBufferSize);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateIndexBuffer() {
    try {
        VkDeviceSize indexBufferSize = sizeof(shapeIndices[0]) * shapeIndices.size();

        auto stagingBuffer = Buffer();
        auto stagingBufferMemory = DeviceMemory();
        CreateBuffer(
            indexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void* data = nullptr;
        auto stagingBufferMemoryMapInfo = GenerateMemoryMapInfo(stagingBufferMemory, indexBufferSize, 0);
        stagingBufferMemory.Map(stagingBufferMemoryMapInfo, &data);
        
        std::memcpy(data, shapeIndices.data(), static_cast<size_t>(indexBufferSize));

        auto stagingBufferMemoryUnmapInfo = GenerateMemoryUnmapInfo(stagingBufferMemory);
        stagingBufferMemory.Unmap(stagingBufferMemoryUnmapInfo);

        _indexBuffer = std::make_unique<Buffer>();
        _indexBufferMemory = std::make_unique<DeviceMemory>();
        CreateBuffer(
            indexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            *_indexBuffer,
            *_indexBufferMemory
        );

        CopyBuffer(stagingBuffer, *_indexBuffer, indexBufferSize);
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

        auto uniformBufferMemoryMapInfo = GenerateMemoryMapInfo(_uniformBuffersMemory[i], uniformBufferSize, 0);
        _uniformBuffersMemory[i].Map(uniformBufferMemoryMapInfo, &_uniformBuffersMapped[i]);
    }
}

void Application::CreateDescriptorPool() {
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
        _descriptorSets = DescriptorSets(descriptorSetAllocateInfo, *_device, *_descriptorPool);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            auto descriptorBufferInfo = GenerateDescriptorBufferInfo(_uniformBuffers[i], sizeof(UniformBufferObject));

            auto writeBufferDescriptorSet = GenerateWriteDescriptorSet(
                1,
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                _descriptorSets.Handles()[i],
                0,
                0,
                &descriptorBufferInfo
            );

            vkUpdateDescriptorSets(_device->Handle(), 1, &writeBufferDescriptorSet, 0, nullptr);
        }
    }
    
    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CreateCommandBuffers() {
    _commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    auto commandBufferAllocateInfo = GenerateCommandBufferAllocateInfo(*_commandPool, static_cast<uint32_t>(_commandBuffers.size())); // create command buffer
    
    try {
        _commandBuffers = AllocateCommandBuffers(commandBufferAllocateInfo, *_device);
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
        WaitForFences(*_device, fencesToResetAndWaitFor);

        uint32_t imageIndex = 0;
        Semaphore& acquireSemaphore = _imageAvailableSemaphores[_frameIndex];
        CommandBuffer& commandBuffer = _commandBuffers[_frameIndex];

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
        VkResult acquireNextImageResult = vkAcquireNextImage2KHR(_device->Handle(), &acquireNextImageInfo, &imageIndex); // acquire next frame
        if (acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR) {
            std::cerr << "Swap chain image is out of date" << std::endl;

            RecreateSwapchain();
            return;
        }

        else if (acquireNextImageResult != VK_SUCCESS && acquireNextImageResult != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Could not aquire swapchain image");
        }

        UpdateUniformBuffer(_frameIndex);

        ResetFences(*_device, fencesToResetAndWaitFor);
        
        // REALLY IMPORTANT: USE SWAPCHAIN IMAGE INDEX RETRIEVED FROM ANI TO PROPERLY RE-USE SEMAPHORES
        Semaphore& submitSemaphore = _renderFinishedSemaphores[imageIndex];

        commandBuffer.Reset();
        RecordCommandBuffer(_commandBuffers[_frameIndex], imageIndex);

        std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfo = { GenerateCommandBufferSubmitInfo(commandBuffer) }; // specify command buffer submit info for submission
        std::vector<VkSemaphoreSubmitInfo> signalSemaphoreSubmitInfo = { GenerateSemaphoreSubmitInfo(submitSemaphore, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT) };
        std::vector<VkSemaphore> signalSemaphores = { submitSemaphore.Handle() };
        std::vector<VkSemaphoreSubmitInfo> waitSemaphoreSubmitInfo = { GenerateSemaphoreSubmitInfo(acquireSemaphore) }; // set synchronization handles behaviours for submission
        std::vector<VkSemaphore> waitSemaphores = { acquireSemaphore.Handle() };

        VkSubmitInfo2 submitInfo = GenerateSumbitInfo(commandBufferSubmitInfo, signalSemaphoreSubmitInfo, waitSemaphoreSubmitInfo);
        VkResult queueSubmitResult = vkQueueSubmit2(_graphicsQueue->Handle(), 1, &submitInfo, frameFence.Handle()); // submit command buffer to the queue
        if (queueSubmitResult != VK_SUCCESS) {
            std::string error = "Unable to submit to queue (status " + std::to_string(queueSubmitResult) + ")";
            throw std::runtime_error(error);
        }

        std::vector<uint32_t> imageIndices = { imageIndex };
        std::vector<VkSwapchainKHR> swapchains = { _swapchain->Handle() };

        VkPresentInfoKHR presentInfo = GeneratePresentInfo(imageIndices, swapchains, signalSemaphores);
        VkResult queuePresentResult = vkQueuePresentKHR(_presentQueue->Handle(), &presentInfo); // present
        
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

    std::vector<VkClearValue> clearValue = { GenerateClearValue() };
    auto renderPassBeginInfoRenderArea = GenerateRect2D(_swapchainExtent);
    VkRenderPassBeginInfo renderPassBeginInfo = GenerateRenderPassBeginInfo(*_renderPass, _framebuffers[imageIndex], clearValue, renderPassBeginInfoRenderArea);
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
    std::vector<VkDeviceSize> sizes = { sizeof(shapeVertices[0]) * shapeVertices.size() };
    commandBuffer.BindVertexBuffers(0, vertexBuffers, offsets, sizes, {});
    
    commandBuffer.BindIndexBuffers(
        *_indexBuffer,
        0,
        (sizeof(shapeIndices[0]) * shapeIndices.size()),
        VK_INDEX_TYPE_UINT16
    );
    std::vector<VkDescriptorSet> descriptorSetsToBind = { _descriptorSets.Handles()[_frameIndex] };
    auto bindDescriptorSetsInfo = GenerateBindDescriptorSetsInfo(
        *_pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        descriptorSetsToBind,
        {}
    );
    commandBuffer.BindDescriptorSets(bindDescriptorSetsInfo);

    commandBuffer.DrawIndexed(static_cast<uint32_t>(shapeIndices.size()), 1, 0, 0, 0);

    auto subpassEndInfo = GenerateSubpassEndInfo();
    commandBuffer.EndRenderPass(subpassEndInfo); // end render pass

    commandBuffer.End();
}

void Application::UpdateUniformBuffer(uint32_t currentImage) {
    static bool logged = false;

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    float ratio = _swapchainExtent.width / static_cast<float>(_swapchainExtent.height); 

    UniformBufferObject ubo {};
    //ubo.model = Math::Matrix4x4::RotateZ(time / 2);
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.model *= glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    ubo.view = Math::Matrix4x4::Transpose(Math::Matrix4x4::LookAt(
        Math::Vector3(2.0f, 2.0f, 2.0f),
        Math::Vector3(0.0f, 0.0f, 0.0f),
        Math::Vector3(0.0f, 0.0f, 1.0f)
    )) * Math::Matrix4x4(-1.0f, 1.0f, -1.0f, 1.0f);
    auto view = Math::Matrix4x4::Transpose(Math::Matrix4x4::LookAt(
        Math::Vector3(2.0f, 2.0f, 2.0f),
        Math::Vector3(0.0f, 0.0f, 0.0f),
        Math::Vector3(0.0f, 0.0f, 1.0f)
    )) * Math::Matrix4x4(-1.0f, 1.0f, -1.0f, 1.0f);
    //ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //ubo.projection = Math::Matrix4x4::Perspective(std::numbers::pi_v<float> / 4, ratio, 0.1f, 10.0f);
    auto proj = Math::Matrix4x4::Transpose(Math::Matrix4x4::Perspective(std::numbers::pi_v<float> / 4, ratio, 0.1f, 10.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 10.0f);

    if (!logged) {
        logged = true;

        std::cout << "Own look at matrix: " << std::endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << std::showpos << std::fixed << std::setprecision(2) << view(i, j) << " ";                
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "GLM's look at matrix: " << std::endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << std::showpos << std::fixed << std::setprecision(2) << ubo.view(i, j) << " ";                
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Own perspective matrix: " << std::endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << std::showpos << std::fixed << std::setprecision(2) << proj(i, j) << " ";                
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "GLM's perspective matrix: " << std::endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << std::showpos << std::fixed << std::setprecision(2) << ubo.proj[i][j] << " ";                
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    //ubo.projection(1, 1) *= -1;
    ubo.proj[1][1] *= -1;

    ubo.time = time;

    std::cout << time << std::endl;

    std::memcpy(_uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

QueueFamilyIndices Application::FindQueueFamilies(PhysicalDevice const& physicalDevice) {
    QueueFamilyIndices indices {};

    auto properties = physicalDevice.QueueFamilyProperties(); // get physical device's queue family properties

    int queueFamilyIndex = 0;
    //std::clog << " - Queue families (count: " << physicalDeviceQueueFamilyProperties.size() << ", "  << physicalDeviceProperties.properties.deviceName << ")" << std::endl;
    for (auto const& queueFamilyProperties : properties) {
        if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = queueFamilyIndex; 
        }

        if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            indices.transferFamily = queueFamilyIndex;
        }

        // check that the retrieved surface is supported by a specific queue family for the current physical device
        bool supportedSurface = physicalDevice.IsSurfaceSupportedByQueueFamily(*_surface, queueFamilyIndex);
        if (supportedSurface) {
            indices.presentFamily = queueFamilyIndex;
        }

        if (indices.IsComplete()) {
            break;
        }

        ++queueFamilyIndex;
    }

    return indices;
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

    return queueFamilyIndices.IsComplete() && deviceExtensionsAreSupported && swapchainIsAdequate;
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
    Buffer& buffer,
    DeviceMemory& bufferMemory
) {
    try {
        VkBufferCreateInfo bufferCreateInfo = GenerateBufferCreateInfo(size, usage, sharingMode);
        buffer = Buffer(bufferCreateInfo, *_device); // will be moved

        auto bufferMemoryRequirementsInfo = GenerateBufferMemoryRequirementsInfo(buffer);
        auto bufferMemoryRequirements = buffer.MemoryRequirements(bufferMemoryRequirementsInfo);

        auto bufferMemoryAllocateInfo = GenerateMemoryAllocateInfo(
            bufferMemoryRequirements.memoryRequirements.size,
            FindMemoryType(
                bufferMemoryRequirements.memoryRequirements.memoryTypeBits,
                properties
            )
        );
        bufferMemory = DeviceMemory(bufferMemoryAllocateInfo, *_device); // will be moved

        std::vector<VkBindBufferMemoryInfo> bufferBindMemoryInfos = { GenerateBindBufferMemoryInfo(bufferMemory, 0, buffer) };
        _device->BindBufferMemory(bufferBindMemoryInfos);
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CopyBuffer(Buffer& src, Buffer& dst, VkDeviceSize size) {
    try {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = GenerateCommandBufferAllocateInfo(*_commandPool);
        auto commandBuffer = AllocateCommandBuffer(commandBufferAllocateInfo, *_device);
        
        VkCommandBufferBeginInfo commandBufferBeginInfo = GenerateCommandBufferBeginInfo();
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        commandBuffer.Begin(commandBufferBeginInfo);

        std::vector<VkBufferCopy2> bufferCopyRegions = { GenerateBufferCopy(size) };
        auto copyBufferInfo = GenerateCopyBufferInfo(src, dst, bufferCopyRegions);
        commandBuffer.CopyBuffer(copyBufferInfo);

        commandBuffer.End();

        std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfo = { GenerateCommandBufferSubmitInfo(commandBuffer) };
        std::vector<VkSubmitInfo2> submitInfo = { GenerateSumbitInfo(commandBufferSubmitInfo, {}, {}) };
        VkResult queueSubmitResult = vkQueueSubmit2(_graphicsQueue->Handle(), 1, submitInfo.data(), VK_NULL_HANDLE);
        if (queueSubmitResult != VK_SUCCESS) {
            std::string error = "Could not submit copy buffer command: (result: code " + std::to_string(queueSubmitResult) + ")";
            throw std::runtime_error(error);
        }

        VkResult queueWaitIdle = vkQueueWaitIdle(_graphicsQueue->Handle());
        if (queueWaitIdle != VK_SUCCESS) {
            std::string error = "Could not wait on graphics queue: (result: code " + std::to_string(queueSubmitResult) + ")";
            throw std::runtime_error(error);
        }

        vkFreeCommandBuffers(_device->Handle(), _commandPool->Handle(), 1, commandBuffer.HandleAddress());
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void Application::CleanupSwapchain() {
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
    CreateFramebuffers();
}
