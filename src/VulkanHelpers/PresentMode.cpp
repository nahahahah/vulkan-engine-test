#include "VulkanHelpers/PresentMode.hpp"

std::vector<VkPresentModeKHR> EnumeratePresentModes(PhysicalDevice const& physicalDevice, Surface const& surface) {
	uint32_t count = 0;
	VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.Handle(), surface.Handle(), &count, VK_NULL_HANDLE);
	if (result != VK_SUCCESS) {
		std::string error = "Could not get physical device surface present modes (1st call, status: " + std::to_string(result) + ")";
		throw std::runtime_error(error);
	}
	std::clog << "Physical device surface present modes retrieved successfully (1st call, count: " << count << ")" << std::endl;

	if (count == 0) {
		std::string error = "No present modes found";
		throw std::runtime_error(error);
	}

	std::vector<VkPresentModeKHR> presentModes(count, VkPresentModeKHR {});
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.Handle(), surface.Handle(), &count, presentModes.data());
	if (result != VK_SUCCESS) {
		std::string error = "Could not get physical device surface present modes (2nd call, status: " + std::to_string(result) + ")";
		throw std::runtime_error(error);
	}
	std::clog << "Physical device surface present modes retrieved successfully (2nd call, retrieved in array)" << std::endl;

	return presentModes;
}

void DisplayPresentMode(VkPresentModeKHR presentMode) {
    std::clog << "\t\t - ";
    switch (presentMode) {
        case VK_PRESENT_MODE_IMMEDIATE_KHR: {
			std::clog << "PRESENT_MODE_IMMEDIATE_KHR";
			break;
		}

        case VK_PRESENT_MODE_MAILBOX_KHR: {
			std::clog << "PRESENT_MODE_MAILBOX_KHR";
			break;
		}

        case VK_PRESENT_MODE_FIFO_KHR: {
			std::clog << "PRESENT_MODE_FIFO_KHR";
			break;
		}

        case VK_PRESENT_MODE_FIFO_RELAXED_KHR: {
			std::clog << "PRESENT_MODE_FIFO_RELAXED_KHR";
			break;
		}

        case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR: {
			std::clog << "PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
			break;
		}

        case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR: {
			std::clog << "PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
			break;
		}

        /*
        case VK_PRESENT_MODE_FIFO_LATEST_READY_KHR: {
			std::clog << "";
			break;
		}
        */ 

        default: {
            std::clog << "Unknown (" << presentMode << ")";
            break;
        }
    }
    std::clog << std::endl;
}
