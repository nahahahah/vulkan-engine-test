#include "VulkanHelpers/ParameterStructs/PhysicalDeviceProperties.hpp"

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceProperties2 const& physicalDeviceProperties) {
    Version apiVersion(physicalDeviceProperties.properties.apiVersion);
    out << " - API version: " << apiVersion << std::endl; 
    out << " - Device ID: " << physicalDeviceProperties.properties.deviceID << std::endl; 
    out << " - Device name: " << physicalDeviceProperties.properties.deviceName << std::endl; 

    out << " - Device type: ";
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
    out << " - Driver version: " << driverVersion << std::endl; 
    out << " - Limits:" << std::endl;
    out << physicalDeviceProperties.properties.limits << std::endl;

    out << std::hex << " - Pipeline cache UUID: ";
    for (int i = 0; i < 16; ++i) {
        out << std::setw(2) << std::setfill('0') << static_cast<int>(physicalDeviceProperties.properties.pipelineCacheUUID[i]) << (i != 15 ? ":" : "");
    }
    out << std::dec << std::endl;;
    
    out << " - Sparse properties:" << std::endl;
    out << physicalDeviceProperties.properties.sparseProperties << std::endl;

    out << " - Vendor ID: " << physicalDeviceProperties.properties.vendorID << std::endl;

    return out;
}
