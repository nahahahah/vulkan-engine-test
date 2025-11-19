#include "VulkanHelpers/CompositeAlphaFlags.hpp"

void DisplayCompositeAlphaFlags(VkCompositeAlphaFlagsKHR flags) {
    if (flags & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)          std::clog << " - COMPOSITE_ALPHA_OPAQUE_BIT_KHR"          << std::endl;
    if (flags & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)  std::clog << " - COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR"  << std::endl;
    if (flags & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) std::clog << " - COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR" << std::endl;
    if (flags & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)         std::clog << " - COMPOSITE_ALPHA_INHERIT_BIT_KHR"         << std::endl;
}
