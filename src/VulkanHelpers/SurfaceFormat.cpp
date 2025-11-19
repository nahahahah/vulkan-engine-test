#include "VulkanHelpers/SurfaceFormat.hpp"

void DisplaySurfaceFormat(VkSurfaceFormatKHR const& surfaceFormat) {
    std::clog << surfaceFormat.colorSpace << std::endl;
    std::clog << "\t\t\t - Format          "
              << surfaceFormat.format << std::endl;
}
