#include "VulkanHelpers/ImageUsageFlags.hpp"

void DisplayImageUsageFlags(VkImageUsageFlags flags) {
    if (flags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)                            std::clog << " - IMAGE_USAGE_TRANSFER_SRC_BIT"                            << std::endl;
    if (flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)                            std::clog << " - IMAGE_USAGE_TRANSFER_DST_BIT"                            << std::endl;
    if (flags & VK_IMAGE_USAGE_SAMPLED_BIT)                                 std::clog << " - IMAGE_USAGE_SAMPLED_BIT"                                 << std::endl;
    if (flags & VK_IMAGE_USAGE_STORAGE_BIT)                                 std::clog << " - IMAGE_USAGE_STORAGE_BIT"                                 << std::endl;
    if (flags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)                        std::clog << " - IMAGE_USAGE_COLOR_ATTACHMENT_BIT"                        << std::endl;
    if (flags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)                std::clog << " - IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT"                << std::endl;
    if (flags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)                    std::clog << " - IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT"                    << std::endl;
    if (flags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)                        std::clog << " - IMAGE_USAGE_INPUT_ATTACHMENT_BIT"                        << std::endl;
    if (flags & VK_IMAGE_USAGE_HOST_TRANSFER_BIT)                           std::clog << " - IMAGE_USAGE_HOST_TRANSFER_BIT"                           << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR)                    std::clog << " - IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR"                    << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR)                    std::clog << " - IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR"                    << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR)                    std::clog << " - IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR"                    << std::endl;
    if (flags & VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT)                std::clog << " - IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT"                << std::endl;
    if (flags & VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR)    std::clog << " - IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR"    << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR)                    std::clog << " - IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR"                    << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR)                    std::clog << " - IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR"                    << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR)                    std::clog << " - IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR"                    << std::endl;
    if (flags & VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT)            std::clog << " - IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT"            << std::endl;
    if (flags & VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI)                  std::clog << " - IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI"                  << std::endl;
    if (flags & VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM)                      std::clog << " - IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM"                      << std::endl;
    if (flags & VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM)                 std::clog << " - IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM"                 << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_QUANTIZATION_DELTA_MAP_BIT_KHR) std::clog << " - IMAGE_USAGE_VIDEO_ENCODE_QUANTIZATION_DELTA_MAP_BIT_KHR" << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_EMPHASIS_MAP_BIT_KHR)           std::clog << " - IMAGE_USAGE_VIDEO_ENCODE_EMPHASIS_MAP_BIT_KHR"           << std::endl;
    if (flags & VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV)                   std::clog << " - IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV"                   << std::endl;
    if (flags & VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT)                       std::clog << " - IMAGE_USAGE_HOST_TRANSFER_BIT_EXT"                       << std::endl;
}
