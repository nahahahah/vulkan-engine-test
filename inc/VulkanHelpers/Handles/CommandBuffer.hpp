#ifndef VK_WRAPPER_COMMAND_BUFFER_HPP
#define VK_WRAPPER_COMMAND_BUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <span>
#include <cassert>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/CommandPool.hpp"
#include "VulkanHelpers/Handles/Device.hpp"
#include "VulkanHelpers/Handles/Pipeline.hpp"
#include "VulkanHelpers/Handles/Buffer.hpp"

class CommandBuffer {
    public:
        CommandBuffer() = delete;
        CommandBuffer(VkCommandBuffer commandBuffer, std::string const& label);
        CommandBuffer(
            VkCommandBufferAllocateInfo const& allocateInfo,
            Device const& device,
            CommandPool const& commandPool,
            std::string const& label
        );
        CommandBuffer(CommandBuffer const& other) = delete;
        CommandBuffer(CommandBuffer&& other);
        ~CommandBuffer();

        CommandBuffer& operator = (CommandBuffer const& other) = delete;
        CommandBuffer& operator = (CommandBuffer&& other);

        std::string Label() { return _label; }
        void Label(std::string const& label) { _label = label; }

        VkCommandBuffer Handle() { return _handle; }
        VkCommandBuffer Handle() const { return _handle; }

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
        void CopyBufferToImage(VkCopyBufferToImageInfo2 const& copyBufferToImageInfo);
        void SetViewport(uint32_t first, uint32_t count, std::span<VkViewport> viewports);
        void SetScissor(uint32_t first, uint32_t count, std::span<VkRect2D> scissors);
        void PipelineBarrier(VkDependencyInfo const& dependencyInfo);
        void BlitImage(VkBlitImageInfo2 const& blitImageInfo);
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
        std::string _label = "";
        VkCommandBuffer _handle = VK_NULL_HANDLE;
        Device const* _device = nullptr;
        CommandPool const* _commandPool = nullptr;
};

class CommandBufferCollection {
    public:
        using Iterator = std::vector<CommandBuffer>::iterator;
        using ConstIterator = std::vector<CommandBuffer>::const_iterator;
        using ReverseIterator = std::vector<CommandBuffer>::reverse_iterator;
        using ConstReverseIterator = std::vector<CommandBuffer>::const_reverse_iterator;

        CommandBufferCollection() = default;
        CommandBufferCollection(VkCommandBufferAllocateInfo const& allocateInfo, Device const& device, std::string const& label);
        CommandBufferCollection(CommandBufferCollection const& other) = delete;
        CommandBufferCollection(CommandBufferCollection&& other) = default;
        /**
         * @note Nothing to destroy specifically. The command buffers will be freed internally by the command pool which was used to allocate them.
         */
        ~CommandBufferCollection() = default;

        CommandBufferCollection& operator = (CommandBufferCollection const& other) = delete;
        CommandBufferCollection& operator = (CommandBufferCollection&& other) = default;

        CommandBuffer& operator [] (size_t index);
        CommandBuffer const& operator [] (size_t index) const;

        size_t size() { return _wrappers.size(); }

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        void Label(std::string const& label) { _label = label; }

        CommandBuffer* Wrappers() { return _wrappers.data(); }
        CommandBuffer const* Wrappers() const { return _wrappers.data(); }
        VkCommandBuffer* Handles() { return _handles.data(); }
        VkCommandBuffer const* Handles() const { return _handles.data(); }

        Iterator begin() { return _wrappers.begin(); }
        ConstIterator begin() const { return _wrappers.begin(); }
        
        Iterator end() { return _wrappers.end(); }
        ConstIterator end() const { return _wrappers.end(); }
        
        ConstIterator cbegin() const { return _wrappers.cbegin(); }
        ConstIterator cend() const { return _wrappers.cend(); }
        
        ReverseIterator rbegin() { return _wrappers.rbegin(); }
        ConstReverseIterator rbegin() const { return _wrappers.rbegin(); }
        
        ReverseIterator rend() { return _wrappers.rend(); }
        ConstReverseIterator rend() const{ return _wrappers.rend(); }
        
        ConstReverseIterator crbegin() const { return _wrappers.crbegin(); }
        ConstReverseIterator crend() const { return _wrappers.crend(); }

    private:
        std::string _label = "";
        std::vector<VkCommandBuffer> _handles {};
        std::vector<CommandBuffer> _wrappers {};
};

#endif // VK_WRAPPER_COMMAND_BUFFER_HPP
