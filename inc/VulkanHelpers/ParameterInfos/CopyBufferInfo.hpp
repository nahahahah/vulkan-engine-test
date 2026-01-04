#ifndef VK_WRAPPER_COPY_BUFFER_INFO_HPP
#define VK_WRAPPER_COPY_BUFFER_INFO_HPP

#include <span>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Buffer.hpp"

VkCopyBufferInfo2 GenerateCopyBufferInfo(Buffer const& src, Buffer const& dst, std::span<VkBufferCopy2> regions);

#endif // VK_WRAPPER_COPY_BUFFER_INFO_HPP
