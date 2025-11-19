#include "VulkanHelpers/SampleCountFlags.hpp"

void DisplaySampleCountFlags(VkSampleCountFlags flags) {
    if (flags &  VK_SAMPLE_COUNT_1_BIT) std::clog << " - SAMPLE_COUNT_1_BIT"  << std::endl;
    if (flags &  VK_SAMPLE_COUNT_2_BIT) std::clog << " - SAMPLE_COUNT_2_BIT"  << std::endl;
    if (flags &  VK_SAMPLE_COUNT_4_BIT) std::clog << " - SAMPLE_COUNT_4_BIT"  << std::endl;
    if (flags &  VK_SAMPLE_COUNT_8_BIT) std::clog << " - SAMPLE_COUNT_8_BIT"  << std::endl;
    if (flags & VK_SAMPLE_COUNT_16_BIT) std::clog << " - SAMPLE_COUNT_16_BIT" << std::endl;
    if (flags & VK_SAMPLE_COUNT_32_BIT) std::clog << " - SAMPLE_COUNT_32_BIT" << std::endl;
    if (flags & VK_SAMPLE_COUNT_64_BIT) std::clog << " - SAMPLE_COUNT_64_BIT" << std::endl;
}
