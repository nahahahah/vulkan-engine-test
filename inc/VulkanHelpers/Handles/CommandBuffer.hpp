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
#include "VulkanHelpers/Handles/Buffer.hpp"

class CommandBuffer {
    public:
        CommandBuffer() = default;
        CommandBuffer(CommandBuffer const&& other) = delete;
        CommandBuffer(CommandBuffer&& other);
        CommandBuffer(VkCommandBuffer commandBuffer);

        VkCommandBuffer Handle() { return _handle; }
        VkCommandBuffer Handle() const { return _handle; }
        VkCommandBuffer* HandleAddress() { return &_handle; }
        VkCommandBuffer const* HandleAddress() const { return &_handle; }

        void Reset(VkCommandBufferResetFlags flags = 0);
        void Begin(VkCommandBufferBeginInfo const& beginInfo);
        void BeginRenderPass(VkRenderPassBeginInfo const& renderPassBeginInfo, VkSubpassBeginInfo const& subpassBeginInfo);
        void BindPipeline(VkPipelineBindPoint bindpoint, Pipeline const& pipeline);
        void BindVertexBuffers(
            uint32_t firstBinding,
            std::span<Buffer*> vertexBuffers,
            std::span<VkDeviceSize> offsets,
            std::span<VkDeviceSize> sizes,
            std::span<VkDeviceSize> strides
        );
        void BindIndexBuffers(
            Buffer& indexBuffer,
            VkDeviceSize offset,
            VkDeviceSize size,
            VkIndexType indexType
        );
        void BindDescriptorSets(VkBindDescriptorSetsInfo const& bindInfo);
        void CopyBuffer(VkCopyBufferInfo2 const& copyInfo);
        void SetViewport(uint32_t first, uint32_t count, std::span<VkViewport> viewports);
        void SetScissor(uint32_t first, uint32_t count, std::span<VkRect2D> scissors);
        void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        void DrawIndexed(
            uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t firstIndex,
            int32_t vertexOffset,
            uint32_t firstInstance
        );
        void EndRenderPass(VkSubpassEndInfo const& endInfo);
        void End();

    private:
        VkCommandBuffer _handle = VK_NULL_HANDLE;
};

/// TODO: EDIT THIS CRAP!
class CommandBuffers {
    public:
        CommandBuffers(VkCommandBufferAllocateInfo const& allocateInfo, Device& device, CommandPool& commandPool) : _device(&device), _commandPool(&commandPool) {
            std::vector<VkCommandBuffer> commandBuffers(allocateInfo.commandBufferCount);
            VkResult result = vkAllocateCommandBuffers(device.Handle(), &allocateInfo, commandBuffers.data());
            if (result != VK_SUCCESS) {
                std::string error = "Unable to allocate " + std::to_string(allocateInfo.commandBufferCount) + " command buffers (status: " + std::to_string(result) + ")";
                throw std::runtime_error(error);
            }
            std::clog << commandBuffers.size() << " command buffers allocated successfully" << std::endl;

            _wrappers.reserve(allocateInfo.commandBufferCount);
            for (int i = 0; i < static_cast<int>(allocateInfo.commandBufferCount); ++i) {
                _wrappers.emplace_back(commandBuffers[i]);
            }
        }

        ~CommandBuffers() {
            std::vector<VkCommandBuffer> handles;
            handles.reserve(_wrappers.size());
            for (auto& wrapper : _wrappers) {
                handles.push_back(wrapper.Handle());
            }

            vkFreeCommandBuffers(
                _device->Handle(),
                _commandPool->Handle(),
                static_cast<uint32_t>(handles.size()),
                handles.data()
            );

            std::clog << "Destroyed " << handles.size() << " command buffers" << std::endl;
        }

    private:
        std::vector<CommandBuffer> _wrappers {};
        CommandPool* _commandPool = nullptr;
        Device* _device = nullptr;
};

CommandBuffer AllocateCommandBuffer(VkCommandBufferAllocateInfo const& allocateInfo, Device const& device);
std::vector<CommandBuffer> AllocateCommandBuffers(VkCommandBufferAllocateInfo const& allocateInfo, Device const& device);

#endif // VK_WRAPPER_COMMAND_BUFFER_HPP
