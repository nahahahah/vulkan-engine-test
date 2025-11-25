#include "VulkanHelpers/ParameterStructs/QueueFamilyProperties.hpp"

VkQueueFamilyProperties2 GenerateQueueFamilyProperties() {
    VkQueueFamilyProperties2 queueFamilyProperties {};
    queueFamilyProperties.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
    queueFamilyProperties.queueFamilyProperties = {};
    queueFamilyProperties.pNext = VK_NULL_HANDLE;

    return queueFamilyProperties;
}

std::vector<VkQueueFamilyProperties2> EnumerateQueueFamilyProperties(PhysicalDevice const& physicalDevice) {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice.Handle(), &count, VK_NULL_HANDLE);
    if (count == 0) {
        std::string error = "Could not find any queue family properties";
        throw std::runtime_error(error);
    }

    std::vector<VkQueueFamilyProperties2> properties(count, GenerateQueueFamilyProperties());
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice.Handle(), &count, properties.data());

    return properties;
}

std::ostream& operator << (std::ostream& out, VkQueueFamilyProperties2 const& queueFamilyProperties) {
    out << " - Min image transfer granularity width:" << std::endl;
    out << " - Width      "
        << queueFamilyProperties.queueFamilyProperties.minImageTransferGranularity.width << std::endl;
    out << " - Height     "
        << queueFamilyProperties.queueFamilyProperties.minImageTransferGranularity.height << std::endl;
    out << " - Depth      "
        << queueFamilyProperties.queueFamilyProperties.minImageTransferGranularity.depth << std::endl;
    out << " - Queue count              "
        << queueFamilyProperties.queueFamilyProperties.queueCount << std::endl;
    out << " - Queue flags:" << std::endl;
    DisplayQueueFlags(queueFamilyProperties.queueFamilyProperties.queueFlags);
    out << " - Timestamp valid bits     "
        << queueFamilyProperties.queueFamilyProperties.timestampValidBits << std::endl;

    return out;
}
