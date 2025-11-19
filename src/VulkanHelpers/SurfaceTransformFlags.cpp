#include "VulkanHelpers/SurfaceTransformFlags.hpp"

void DisplaySurfaceTransformFlagsKHR(VkSurfaceTransformFlagsKHR flags) {
    if (flags &          VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_OPAQUE_BIT_KHR"          << std::endl;
    if (flags &  VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR"  << std::endl;
    if (flags & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR" << std::endl;
    if (flags &         VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_INHERIT_BIT_KHR"         << std::endl;
}
