#include "VulkanHelpers/Handles/Device.hpp"

#include "VulkanHelpers/Handles/Swapchain.hpp"
#include "VulkanHelpers/Handles/Image.hpp"

Device::Device(VkDeviceCreateInfo const& createInfo, PhysicalDevice const& physicalDevice) {
    VkResult result = vkCreateDevice(physicalDevice.Handle(), &createInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to create a device (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Device created successully: <VkDevice " << _handle << ">" << std::endl;
}

Device::Device(Device&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;
}

Device::~Device() {
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyDevice(_handle, VK_NULL_HANDLE);
        std::clog << "Device destroyed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

Device& Device::operator = (Device&& other) {
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    return *this;
}

void Device::WaitIdle() {
    VkResult result = vkDeviceWaitIdle(_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to wait for idleing of the device (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Device is now idle" << std::endl;
}

VkResult Device::AcquireNextImage(VkAcquireNextImageInfoKHR const& acquireNextImageInfo, uint32_t* imageIndex) {
    return vkAcquireNextImage2KHR(_handle, &acquireNextImageInfo, imageIndex);
}


void Device::WaitForFences(std::span<VkFence> const& fences, VkBool32 waitAll, uint64_t timeout) {
    VkResult result = vkWaitForFences(_handle, static_cast<uint32_t>(fences.size()), fences.data(), waitAll, timeout);
    if (result != VK_SUCCESS) {
        std::string error = "Could not wait for fences (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Waited for fences successfully" << std::endl;
}

void Device::ResetFences(std::span<VkFence> const& fences) {
    VkResult result = vkResetFences(_handle, static_cast<uint32_t>(fences.size()), fences.data());
    if (result != VK_SUCCESS) {
        std::string error = "Could not reset fences (status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Reset fences successfully" << std::endl;
}

void Device::MapMemory(VkMemoryMapInfo const& memoryMapInfo, void** data) {
    VkResult result = vkMapMemory2(_handle, &memoryMapInfo, data);
    if (result != VK_SUCCESS) {
        std::string error = "Could not map memory (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }

    std::clog << "Memory mapped successfully: <VkDeviceMemory " << _handle << ">" << std::endl;
}

void Device::UnmapMemory(VkMemoryUnmapInfo const& memoryUnmapInfo) {
    VkResult result = vkUnmapMemory2(_handle, &memoryUnmapInfo);
    if (result != VK_SUCCESS) {
        std::string error = "Could not unmap memory (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }

    std::clog << "Memory unmapped successfully: <VkDeviceMemory " << _handle << ">" << std::endl;
}

VkMemoryRequirements2 Device::BufferMemoryRequirements(VkBufferMemoryRequirementsInfo2 const& info) {
    VkMemoryRequirements2 requirements {};

    // structure type
    requirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2; 

    vkGetBufferMemoryRequirements2(_handle, &info, &requirements);

    // extend requirements
    requirements.pNext = VK_NULL_HANDLE;

    return requirements;
}

void Device::BindBufferMemory(std::span<VkBindBufferMemoryInfo> bindInfos) {
    vkBindBufferMemory2(_handle, static_cast<uint32_t>(bindInfos.size()), bindInfos.data());
}

std::vector<Image> Device::SwapchainImages(Device const& device, Swapchain const& swapchain) {
    uint32_t count = 0;
    VkResult result = vkGetSwapchainImagesKHR(device.Handle(), swapchain.Handle(), &count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to retrieve the swap chain images (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Swap chain images retrieved successully (1st call, count: " << count << ")" << std::endl;

    std::vector<VkImage> imagesHandle(count);
    result = vkGetSwapchainImagesKHR(device.Handle(), swapchain.Handle(), &count, imagesHandle.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to retrieve the swap chain images (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    //std::clog << "Swap chain images retrieved successully (2nd call, retrieved in array)" << std::endl;

    std::vector<Image> images {};
    images.reserve(imagesHandle.size());
    for (auto const& imageHandle : imagesHandle) {
        images.emplace_back(imageHandle);
    }

    return images;
}

VkMemoryRequirements2 Device::ImageMemoryRequirements(VkImageMemoryRequirementsInfo2 const& info) {
    VkMemoryRequirements2 requirements {};

    // structure type
    requirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;

    vkGetImageMemoryRequirements2(_handle, &info, &requirements);

    // extend requirements
    requirements.pNext = VK_NULL_HANDLE;

    return requirements;
}

void Device::BindImageMemory(std::span<VkBindImageMemoryInfo> bindInfos) {
    vkBindImageMemory2(_handle, static_cast<uint32_t>(bindInfos.size()), bindInfos.data());
}