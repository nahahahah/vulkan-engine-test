#include "VulkanHelpers/Handles/CommandBuffer.hpp"

CommandBuffer::CommandBuffer(VkCommandBuffer commandBuffer)
    : _handle(commandBuffer) {
}

CommandBuffer::CommandBuffer(
    VkCommandBufferAllocateInfo const& allocateInfo,
    Device const& device,
    CommandPool const* commandPool
) : _device(&device), _commandPool(commandPool) {
    auto safeAllocateInfo = allocateInfo;
    safeAllocateInfo.commandBufferCount = 1;
    VkResult result = vkAllocateCommandBuffers(_device->Handle(), &safeAllocateInfo, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to allocate a single command buffer (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Single command buffer allocated successfully: <VkCommandBuffer " << _handle << ">" << std::endl;
}

CommandBuffer::CommandBuffer(CommandBuffer&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    _commandPool = other._commandPool;
    other._commandPool = nullptr;
}

CommandBuffer::~CommandBuffer() {
    if (_commandPool != nullptr && _commandPool->Handle() != VK_NULL_HANDLE && _handle != nullptr) {
        vkFreeCommandBuffers(_device->Handle(), _commandPool->Handle(), 1, &_handle);
        _handle = nullptr;
    }
}

CommandBuffer& CommandBuffer::operator = (CommandBuffer&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    _commandPool = other._commandPool;
    other._commandPool = nullptr;

    return *this;
}

void CommandBuffer::Reset(VkCommandBufferResetFlags flags) {
    VkResult result = vkResetCommandBuffer(_handle, flags);
    if (result != VK_SUCCESS) {
        std::string error = "Could not reset command buffer (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Reset command buffer successfully" << std::endl;
}

void CommandBuffer::Begin(VkCommandBufferBeginInfo const& beginInfo) {
    VkResult result = vkBeginCommandBuffer(_handle, &beginInfo);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to begin command buffer recording (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Command buffer recording began successfully: <VkCommandBuffer " << commandBuffer << ">" << std::endl;
}

void CommandBuffer::BeginRenderPass(VkRenderPassBeginInfo const& renderPassBeginInfo, VkSubpassBeginInfo const& subpassBeginInfo) {
    vkCmdBeginRenderPass2(_handle, &renderPassBeginInfo, &subpassBeginInfo);
}

void CommandBuffer::BindPipeline(VkPipelineBindPoint bindpoint, Pipeline const& pipeline) {
    vkCmdBindPipeline(_handle, bindpoint, pipeline.Handle());
}

void CommandBuffer::BindVertexBuffers(
    uint32_t firstBinding,
    std::span<Buffer*> vertexBuffers,
    std::span<VkDeviceSize> offsets,
    std::span<VkDeviceSize> sizes,
    std::span<VkDeviceSize> strides
) {
    std::vector<VkBuffer> vertexBufferHandles;
    vertexBufferHandles.reserve(vertexBuffers.size());
    for (auto* vertexBuffer : vertexBuffers) {
        vertexBufferHandles.push_back(vertexBuffer->Handle());
    }

    vkCmdBindVertexBuffers2(
        _handle,
        firstBinding,
        static_cast<uint32_t>(vertexBuffers.size()),
        vertexBufferHandles.data(),
        offsets.data(),
        sizes.data(),
        ((strides.size() != 0) ? (strides.data()) : (VK_NULL_HANDLE))
    );
}

void CommandBuffer::BindIndexBuffers(
    Buffer& indexBuffer,
    VkDeviceSize offset,
    VkDeviceSize size,
    VkIndexType indexType
) {
    vkCmdBindIndexBuffer2(
        _handle,
        indexBuffer.Handle(),
        offset,
        size,
        indexType
    );
}

void CommandBuffer::BindDescriptorSets(VkBindDescriptorSetsInfo const& bindInfo) {
    vkCmdBindDescriptorSets2(_handle, &bindInfo);
}

void CommandBuffer::CopyBuffer(VkCopyBufferInfo2 const& copyInfo) {
    vkCmdCopyBuffer2(_handle, &copyInfo);
}


void CommandBuffer::SetViewport(uint32_t first, uint32_t count, std::span<VkViewport> viewports) {
    vkCmdSetViewport(_handle, first, count, viewports.data());
}

void CommandBuffer::SetScissor(uint32_t first, uint32_t count, std::span<VkRect2D> scissors) {
    vkCmdSetScissor(_handle, first, count, scissors.data());
}

void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    vkCmdDraw(_handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

void CommandBuffer::DrawIndexed(
    uint32_t indexCount,
    uint32_t instanceCount,
    uint32_t firstIndex,
    int32_t vertexOffset,
    uint32_t firstInstance
) {
    vkCmdDrawIndexed(_handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void CommandBuffer::EndRenderPass(VkSubpassEndInfo const& endInfo) {
    vkCmdEndRenderPass2(_handle, &endInfo);
}

void CommandBuffer::End() {
    VkResult result = vkEndCommandBuffer(_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to end the command buffer (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Command buffer ended successfully" << std::endl;
}

CommandBufferCollection::CommandBufferCollection(VkCommandBufferAllocateInfo const& allocateInfo, Device const& device) {
    _handles.resize(allocateInfo.commandBufferCount);
    VkResult result = vkAllocateCommandBuffers(device.Handle(), &allocateInfo, _handles.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to allocate command buffers (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Command buffers allocated successfully" << std::endl;

    for (int i = 0; i < static_cast<int>(allocateInfo.commandBufferCount); ++i) {
        _wrappers.emplace_back(_handles[i]);
    }
}

CommandBuffer& CommandBufferCollection::operator [] (size_t index) {
    assert("`index` must be within the bounds of the container" &&
           index < _wrappers.size());

    return _wrappers[index];
}

CommandBuffer const& CommandBufferCollection::operator [] (size_t index) const {
    assert("`index` must be within the bounds of the container" &&
           index < _wrappers.size());

    return _wrappers[index];
}
