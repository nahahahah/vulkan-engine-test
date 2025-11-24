#include "VulkanHelpers/CommandBuffer.hpp"

CommandBuffer::CommandBuffer(VkCommandBuffer commandBuffer)
    : _handle(commandBuffer) {
    std::cout << "==================== [COMMAND BUFFER: " << _handle << "] ====================" << std::endl;
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

void CommandBuffer::SetViewport(uint32_t first, uint32_t count, std::span<VkViewport> viewports) {
    vkCmdSetViewport(_handle, first, count, viewports.data());
}

void CommandBuffer::SetScissor(uint32_t first, uint32_t count, std::span<VkRect2D> scissors) {
    vkCmdSetScissor(_handle, first, count, scissors.data());
}

void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    vkCmdDraw(_handle, vertexCount, instanceCount, firstVertex, firstInstance);
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

std::vector<CommandBuffer> AllocateCommandBuffers(VkCommandBufferAllocateInfo const& allocateInfo, Device const& device) {
    std::vector<VkCommandBuffer> commandBuffers(allocateInfo.commandBufferCount);
    VkResult result = vkAllocateCommandBuffers(device.Handle(), &allocateInfo, commandBuffers.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to allocate a command buffer (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Command buffers allocated successfully" << std::endl;

    std::vector<CommandBuffer> commandBufferHandles {};
    for (int i = 0; i < static_cast<int>(allocateInfo.commandBufferCount); ++i) {
        commandBufferHandles.emplace_back(commandBuffers[i]);
    }

    return commandBufferHandles;
}
