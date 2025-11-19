#include "VulkanHelpers/PhysicalDeviceProperties.hpp"

PhysicalDeviceProperties::PhysicalDeviceProperties(PhysicalDevice const& physicalDevice) {
    this->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    this->properties = {};
    this->pNext = VK_NULL_HANDLE;

    vkGetPhysicalDeviceProperties2(physicalDevice.Handle(), this);
}

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceProperties2 const& physicalDeviceProperties) {
    Version apiVersion(physicalDeviceProperties.properties.apiVersion);
    out << "\t - API version: " << apiVersion << std::endl; 
    out << "\t - Device ID: " << physicalDeviceProperties.properties.deviceID << std::endl; 
    out << "\t - Device name: " << physicalDeviceProperties.properties.deviceName << std::endl; 

    out << "\t - Device type: ";
    switch (physicalDeviceProperties.properties.deviceType) {
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU: {
            out << "CPU";
            break;
        }

        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
            out << "Discrete GPU";
            break;
        }

        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
            out << "Integrated GPU";
            break;
        }

        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER: {
            out << "Other";
            break;
        }
        
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: {
            out << "Virtual GPU";
            break;
        }

        default: {
            out << "Unknown";
            break;
        }
    }
    out << std::endl;

    DriverVersion driverVersion(
          physicalDeviceProperties.properties.driverVersion,
          physicalDeviceProperties.properties.vendorID
    );
    out << "\t - Driver version: " << driverVersion << std::endl; 
    out << "\t - Limits:" << std::endl;
    out << physicalDeviceProperties.properties.limits << std::endl;

    out << std::hex << "\t - Pipeline cache UUID: ";
    for (int i = 0; i < 16; ++i) {
        out << std::setw(2) << std::setfill('0') << static_cast<int>(physicalDeviceProperties.properties.pipelineCacheUUID[i]) << (i != 15 ? ":" : "");
    }
    out << std::dec << std::endl;;
    
    out << "\t - Sparse properties:" << std::endl;
    out << physicalDeviceProperties.properties.sparseProperties << std::endl;

    out << "\t - Vendor ID: " << physicalDeviceProperties.properties.vendorID << std::endl;

    return out;
}
