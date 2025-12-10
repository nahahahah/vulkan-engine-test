#include "VulkanHelpers/ParameterEnums/PresentMode.hpp"

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
