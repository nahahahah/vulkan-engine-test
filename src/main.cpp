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

    auto app = Application();

    try {
        app.Run();

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
        pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE /* VK_CULL_MODE_BACK_BIT */;
        pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        
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
