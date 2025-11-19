#include "VulkanHelpers/QueueFamilyProperties.hpp"

QueueFamilyProperties::QueueFamilyProperties() {
    this->sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
    this->queueFamilyProperties = {};
    this->pNext = VK_NULL_HANDLE;
}

std::vector<QueueFamilyProperties> QueueFamilyProperties::Enumerate(PhysicalDevice const& physicalDevice) {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice.Handle(), &count, VK_NULL_HANDLE);
    if (count == 0) {
        std::string error = "Could not find any queue family properties";
        throw std::runtime_error(error);
    }

    std::vector<QueueFamilyProperties> properties(count, QueueFamilyProperties {});
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice.Handle(), &count, properties.data());

    return properties;
}

void QueueFamilyProperties::Display(QueueFamilyProperties const& queueFamilyProperties) {
    std::clog << "\t\t\t - Min image transfer granularity width:" << std::endl;
    std::clog << "\t\t\t\t - Width      "
              << queueFamilyProperties.queueFamilyProperties.minImageTransferGranularity.width << std::endl;
    std::clog << "\t\t\t\t - Height     "
              << queueFamilyProperties.queueFamilyProperties.minImageTransferGranularity.height << std::endl;
    std::clog << "\t\t\t\t - Depth      "
              << queueFamilyProperties.queueFamilyProperties.minImageTransferGranularity.depth << std::endl;
    std::clog << "\t\t\t - Queue count              "
              << queueFamilyProperties.queueFamilyProperties.queueCount << std::endl;
    std::clog << "\t\t\t - Queue flags:" << std::endl;
    DisplayQueueFlags(queueFamilyProperties.queueFamilyProperties.queueFlags);
    std::clog << "\t\t\t - Timestamp valid bits     "
              << queueFamilyProperties.queueFamilyProperties.timestampValidBits << std::endl;
}
