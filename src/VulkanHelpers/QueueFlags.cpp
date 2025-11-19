#include "VulkanHelpers/QueueFlags.hpp"

void DisplayQueueFlags(VkQueueFlags flags) {
    if (flags &         VK_QUEUE_GRAPHICS_BIT) std::clog << "\t\t\t\t - QUEUE_GRAPHICS_BIT"         << std::endl;
    if (flags &          VK_QUEUE_COMPUTE_BIT) std::clog << "\t\t\t\t - QUEUE_COMPUTE_BIT"          << std::endl;
    if (flags &         VK_QUEUE_TRANSFER_BIT) std::clog << "\t\t\t\t - QUEUE_TRANSFER_BIT"         << std::endl;
    if (flags &   VK_QUEUE_SPARSE_BINDING_BIT) std::clog << "\t\t\t\t - QUEUE_SPARSE_BINDING_BIT"   << std::endl;
    if (flags &        VK_QUEUE_PROTECTED_BIT) std::clog << "\t\t\t\t - QUEUE_PROTECTED_BIT"        << std::endl;
    if (flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) std::clog << "\t\t\t\t - QUEUE_VIDEO_DECODE_BIT_KHR" << std::endl;
    if (flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) std::clog << "\t\t\t\t - QUEUE_VIDEO_ENCODE_BIT_KHR" << std::endl;
    if (flags &  VK_QUEUE_OPTICAL_FLOW_BIT_NV) std::clog << "\t\t\t\t - QUEUE_OPTICAL_FLOW_BIT_NV"  << std::endl;
}
