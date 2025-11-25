#ifndef VK_WRAPPER_COMMAND_BUFFER_HPP
#define VK_WRAPPER_COMMAND_BUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/CommandPool.hpp"
#include "VulkanHelpers/Handles/Device.hpp"
#include "VulkanHelpers/Handles/Pipeline.hpp"

class CommandBuffer {
    public:
        CommandBuffer() = default;
        CommandBuffer(VkCommandBuffer commandBuffer);

        VkCommandBuffer Handle() { return _handle; }
        VkCommandBuffer Handle() const { return _handle; }

        void Reset(VkCommandBufferResetFlags flags);
        void Begin(VkCommandBufferBeginInfo const& beginInfo);
        void BeginRenderPass(VkRenderPassBeginInfo const& renderPassBeginInfo, VkSubpassBeginInfo const& subpassBeginInfo);
        void BindPipeline(VkPipelineBindPoint bindpoint, Pipeline const& pipeline);
        void SetViewport(uint32_t first, uint32_t count, std::span<VkViewport> viewports);
        void SetScissor(uint32_t first, uint32_t count, std::span<VkRect2D> scissors);
        void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        void EndRenderPass(VkSubpassEndInfo const& endInfo);
        void End();

    private:
        VkCommandBuffer _handle = VK_NULL_HANDLE;
};

std::vector<CommandBuffer> AllocateCommandBuffers(VkCommandBufferAllocateInfo const& allocateInfo, Device const& device);

#endif // VK_WRAPPER_COMMAND_BUFFER_HPP
