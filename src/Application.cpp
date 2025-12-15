#include "Application.hpp"

// debug callback
#ifndef NDEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
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

uint32_t FindMemoryType(PhysicalDevice const& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    auto memoryProperties = physicalDevice.MemoryProperties().memoryProperties;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type");
}

std::pair<Buffer, DeviceMemory> CreateBuffer(
    PhysicalDevice const& physicalDevice,
    Device& device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties
) {
    try {
        VkBufferCreateInfo bufferCreateInfo = GenerateBufferCreateInfo(size, usage, sharingMode);
        auto buffer = Buffer(bufferCreateInfo, &device);

        auto bufferMemoryRequirementsInfo = GenerateBufferMemoryRequirementsInfo(buffer);
        auto bufferMemoryRequirements = buffer.MemoryRequirements(bufferMemoryRequirementsInfo);

        auto bufferMemoryAllocateInfo = GenerateMemoryAllocateInfo(
            bufferMemoryRequirements.memoryRequirements.size,
            FindMemoryType(
                physicalDevice,
                bufferMemoryRequirements.memoryRequirements.memoryTypeBits,
                properties
            )
        );
        auto bufferMemory = DeviceMemory(bufferMemoryAllocateInfo, &device);

        std::vector<VkBindBufferMemoryInfo> bufferBindMemoryInfos = { GenerateBindBufferMemoryInfo(bufferMemory, 0, buffer) };
        device.BindBufferMemory(bufferBindMemoryInfos);

        return std::make_pair(std::move(buffer), std::move(bufferMemory));
    }

    catch (std::exception const& e) {
        throw e;
    }
}

std::pair<Buffer, DeviceMemory> CreateVertexBuffer(
    std::span<Vertex> vertices,
    PhysicalDevice const& physicalDevice,
    Device& device,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties,
    uint32_t queueFamilyIndex,
    Queue& graphicsQueue
) {
    try {
        VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
        auto&& [stagingBuffer, stagingBufferMemory] = CreateBuffer(
            physicalDevice,
            device,
            vertexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            sharingMode,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        void* data = nullptr;
        auto stagingBufferMemoryMapInfo = GenerateMemoryMapInfo(stagingBufferMemory, vertexBufferSize, 0);
        stagingBufferMemory.Map(stagingBufferMemoryMapInfo, &data);
        
        std::memcpy(data, vertices.data(), static_cast<size_t>(vertexBufferSize));

        auto stagingBufferMemoryUnmapInfo = GenerateMemoryUnmapInfo(stagingBufferMemory);
        stagingBufferMemory.Unmap(stagingBufferMemoryUnmapInfo);

        auto&& [vertexBuffer, vertexBufferMemory] = CreateBuffer(
            physicalDevice,
            device,
            vertexBufferSize,
            usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            sharingMode,
            properties | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(queueFamilyIndex, device, graphicsQueue, stagingBuffer, vertexBuffer, vertexBufferSize);

        return std::make_pair(std::move(vertexBuffer), std::move(vertexBufferMemory));
    }

    catch (std::exception const& e) {
        throw e;
    }
}

std::pair<Buffer, DeviceMemory> CreateIndexBuffer(
    std::span<uint16_t> indices,
    PhysicalDevice const& physicalDevice,
    Device& device,
    VkBufferUsageFlags usage,
    VkSharingMode sharingMode,
    VkMemoryPropertyFlags properties,
    uint32_t queueFamilyIndex,
    Queue& graphicsQueue
) {
    try {
        VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();
        auto&& [stagingBuffer, stagingBufferMemory] = CreateBuffer(
            physicalDevice,
            device,
            indexBufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            sharingMode,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        void* data = nullptr;
        auto stagingBufferMemoryMapInfo = GenerateMemoryMapInfo(stagingBufferMemory, indexBufferSize, 0);
        stagingBufferMemory.Map(stagingBufferMemoryMapInfo, &data);
        
        std::memcpy(data, indices.data(), static_cast<size_t>(indexBufferSize));

        auto stagingBufferMemoryUnmapInfo = GenerateMemoryUnmapInfo(stagingBufferMemory);
        stagingBufferMemory.Unmap(stagingBufferMemoryUnmapInfo);

        auto&& [vertexBuffer, vertexBufferMemory] = CreateBuffer(
            physicalDevice,
            device,
            indexBufferSize,
            usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            sharingMode,
            properties | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(queueFamilyIndex, device, graphicsQueue, stagingBuffer, vertexBuffer, indexBufferSize);

        return std::make_pair(std::move(vertexBuffer), std::move(vertexBufferMemory));
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void CopyBuffer(uint32_t queueFamilyIndex, Device& device, Queue& graphicsQueue, Buffer& src, Buffer& dst, VkDeviceSize size) {
    try {
        VkCommandPoolCreateInfo commandPoolCreateInfo = GenerateCommandPoolCreateInfo(queueFamilyIndex);
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        auto commandPool = CommandPool(commandPoolCreateInfo, device);

        VkCommandBufferAllocateInfo commandBufferAllocateInfo = GenerateCommandBufferAllocateInfo(commandPool);
        auto commandBuffer = AllocateCommandBuffer(commandBufferAllocateInfo, device);
        
        VkCommandBufferBeginInfo commandBufferBeginInfo = GenerateCommandBufferBeginInfo();
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        commandBuffer.Begin(commandBufferBeginInfo);

        std::vector<VkBufferCopy2> bufferCopyRegions = { GenerateBufferCopy(size) };
        auto copyBufferInfo = GenerateCopyBufferInfo(src, dst, bufferCopyRegions);
        commandBuffer.CopyBuffer(copyBufferInfo);

        commandBuffer.End();

        std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfo = { GenerateCommandBufferSubmitInfo(commandBuffer) };
        std::vector<VkSubmitInfo2> submitInfo = { GenerateSumbitInfo(commandBufferSubmitInfo, {}, {}) };
        VkResult queueSubmitResult = vkQueueSubmit2(graphicsQueue.Handle(), 1, submitInfo.data(), VK_NULL_HANDLE);
        if (queueSubmitResult != VK_SUCCESS) {
            std::string error = "Could not submit copy buffer command: (result: code " + std::to_string(queueSubmitResult) + ")";
            throw std::runtime_error(error);
        }

        VkResult queueWaitIdle = vkQueueWaitIdle(graphicsQueue.Handle());
        if (queueWaitIdle != VK_SUCCESS) {
            std::string error = "Could not wait on graphics queue: (result: code " + std::to_string(queueSubmitResult) + ")";
            throw std::runtime_error(error);
        }

        vkFreeCommandBuffers(device.Handle(), commandPool.Handle(), 1, commandBuffer.HandleAddress());
    }

    catch (std::exception const& e) {
        throw e;
    }
}

void UpdateUniformBuffer(void* mappedUniformBuffer, float ratio) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo {};
    //ubo.model = Math::Matrix4x4::RotateZ(time / 2);
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    /*
    ubo.view = Math::Matrix4x4::LookAt(
        Math::Vector3(2.0f, 2.0f, 2.0f),
        Math::Vector3(0.0f, 0.0f, 0.0f),
        Math::Vector3(0.0f, 0.0f, 1.0f)
    );
    */
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //ubo.projection = Math::Matrix4x4::Perspective(std::numbers::pi_v<float> / 4, ratio, 0.1f, 10.0f);
    ubo.proj = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 10.0f);

    //ubo.projection(1, 1) *= -1;
    ubo.proj[1][1] *= -1;

    std::memcpy(mappedUniformBuffer, &ubo, sizeof(ubo));
}
