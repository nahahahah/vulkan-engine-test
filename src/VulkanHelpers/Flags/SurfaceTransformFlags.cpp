#include "VulkanHelpers/Flags/SurfaceTransformFlags.hpp"

void DisplaySurfaceTransformFlags(VkSurfaceTransformFlagsKHR flags) {
        if (flags & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)                     std::clog << " - SURFACE_TRANSFORM_IDENTITY_BIT_KHR"                     << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR)                    std::clog << " - SURFACE_TRANSFORM_ROTATE_90_BIT_KHR"                    << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR)                   std::clog << " - SURFACE_TRANSFORM_ROTATE_180_BIT_KHR"                   << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)                   std::clog << " - SURFACE_TRANSFORM_ROTATE_270_BIT_KHR"                   << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR)            std::clog << " - SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR"            << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR)  std::clog << " - SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR"  << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR) std::clog << " - SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR" << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR) std::clog << " - SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR" << std::endl;
        if (flags & VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR)                      std::clog << " - SURFACE_TRANSFORM_INHERIT_BIT_KHR"                      << std::endl;
}
