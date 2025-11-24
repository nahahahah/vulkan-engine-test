#include "VulkanHelpers/SubpassEndInfo.hpp"

VkSubpassEndInfo GenerateSubpassEndInfo() {
    VkSubpassEndInfo endInfo {};
    
    // structure type
    endInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO;
    
    // extend end info
    endInfo.pNext = VK_NULL_HANDLE;
    
    return endInfo;
}
