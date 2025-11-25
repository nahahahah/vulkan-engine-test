#include "VulkanHelpers/Handles/Image.hpp"

// TODO: Create a class for VkImage as it's a handle. Not urgent for now.
std::vector<VkImage> EnumerateSwapChainImages(Device const& device, Swapchain const& swapChain) {
    uint32_t count = 0;
    VkResult result = vkGetSwapchainImagesKHR(device.Handle(), swapChain.Handle(), &count, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        std::string error = "Unable to retrieve the swap chain images (1st call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Swap chain images retrieved successully (1st call, count: " << count << ")" << std::endl;

    std::vector<VkImage> images(count);
    result = vkGetSwapchainImagesKHR(device.Handle(), swapChain.Handle(), &count, images.data());
    if (result != VK_SUCCESS) {
        std::string error = "Unable to retrieve the swap chain images (2nd call, status: " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Swap chain images retrieved successully (2nd call, retrieved in array)" << std::endl;

    return images;
}
