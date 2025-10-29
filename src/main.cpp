#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>
#include <iomanip>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <fstream>

#define SDL_MAIN_HANDLED
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#define NVIDIA_DRIVER_VERSION_VARIANT(version) \
    ((uint32_t)(version) >> 22U)

#define NVIDIA_DRIVER_VERSION_MAJOR(version) \
    (((uint32_t)(version) & 0x3FFFFF) >> 14U)

enum class VendorID : uint32_t {
    NVIDIA = 0x10DE
};

struct Version {
    Version() = default;
    Version(uint32_t version)
        : variant(VK_API_VERSION_VARIANT(version))
        ,   major(  VK_API_VERSION_MAJOR(version))
        ,   minor(  VK_API_VERSION_MINOR(version))
        ,   patch(  VK_API_VERSION_PATCH(version)) {}

    friend std::ostream& operator << (std::ostream& out, Version const& v) {
        return (out << v.variant << "." << v.major << "." << v.minor << "." << v.patch);
    }

    int variant = 0;
    int major = 0;
    int minor = 0;
    int patch = 0;
};

struct DriverVersion : public Version {
    DriverVersion() = default;
    DriverVersion(uint32_t version, uint32_t vendorID) {
        switch ((VendorID)(vendorID)) {
            case VendorID::NVIDIA: {
                variant = NVIDIA_DRIVER_VERSION_VARIANT(version);
                major   =   NVIDIA_DRIVER_VERSION_MAJOR(version);
                minor   =                                     0U;
                patch   =                                     0U;
                break;
            }

            default: {
                Version driverVersion(version);
                variant = driverVersion.variant;
                major   =   driverVersion.major;
                minor   =   driverVersion.minor;
                patch   =   driverVersion.patch;
                break;
            }
        }
    }
};

void DisplaySampleCountFlags(VkSampleCountFlags flags) {
    if (flags &  VK_SAMPLE_COUNT_1_BIT) std::clog << "\t\t\t - SAMPLE_COUNT_1_BIT"  << std::endl;
    if (flags &  VK_SAMPLE_COUNT_2_BIT) std::clog << "\t\t\t - SAMPLE_COUNT_2_BIT"  << std::endl;
    if (flags &  VK_SAMPLE_COUNT_4_BIT) std::clog << "\t\t\t - SAMPLE_COUNT_4_BIT"  << std::endl;
    if (flags &  VK_SAMPLE_COUNT_8_BIT) std::clog << "\t\t\t - SAMPLE_COUNT_8_BIT"  << std::endl;
    if (flags & VK_SAMPLE_COUNT_16_BIT) std::clog << "\t\t\t - SAMPLE_COUNT_16_BIT" << std::endl;
    if (flags & VK_SAMPLE_COUNT_32_BIT) std::clog << "\t\t\t - SAMPLE_COUNT_32_BIT" << std::endl;
    if (flags & VK_SAMPLE_COUNT_64_BIT) std::clog << "\t\t\t - SAMPLE_COUNT_64_BIT" << std::endl;
}

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

void DisplaySurfaceTransformFlagsKHR(VkSurfaceTransformFlagsKHR flags) {
    if (flags &          VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_OPAQUE_BIT_KHR"          << std::endl;
    if (flags &  VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR"  << std::endl;
    if (flags & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR" << std::endl;
    if (flags &         VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR) std::clog << "\t\t\t - COMPOSITE_ALPHA_INHERIT_BIT_KHR"         << std::endl;
}

void DisplayImageUsageFlags(VkImageUsageFlags flags) {
    if (flags &                            VK_IMAGE_USAGE_TRANSFER_SRC_BIT) std::clog << "\t\t\t - IMAGE_USAGE_TRANSFER_SRC_BIT"                            << std::endl;
    if (flags &                            VK_IMAGE_USAGE_TRANSFER_DST_BIT) std::clog << "\t\t\t - IMAGE_USAGE_TRANSFER_DST_BIT"                            << std::endl;
    if (flags &                                 VK_IMAGE_USAGE_SAMPLED_BIT) std::clog << "\t\t\t - IMAGE_USAGE_SAMPLED_BIT"                                 << std::endl;
    if (flags &                                 VK_IMAGE_USAGE_STORAGE_BIT) std::clog << "\t\t\t - IMAGE_USAGE_STORAGE_BIT"                                 << std::endl;
    if (flags &                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) std::clog << "\t\t\t - IMAGE_USAGE_COLOR_ATTACHMENT_BIT"                        << std::endl;
    if (flags &                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) std::clog << "\t\t\t - IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT"                << std::endl;
    if (flags &                    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) std::clog << "\t\t\t - IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT"                    << std::endl;
    if (flags &                        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) std::clog << "\t\t\t - IMAGE_USAGE_INPUT_ATTACHMENT_BIT"                        << std::endl;
    if (flags &                           VK_IMAGE_USAGE_HOST_TRANSFER_BIT) std::clog << "\t\t\t - IMAGE_USAGE_HOST_TRANSFER_BIT"                           << std::endl;
    if (flags &                    VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR"                    << std::endl;
    if (flags &                    VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR"                    << std::endl;
    if (flags &                    VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR"                    << std::endl;
    if (flags &                VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT) std::clog << "\t\t\t - IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT"                << std::endl;
    if (flags &    VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR"    << std::endl;
    if (flags &                    VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR"                    << std::endl;
    if (flags &                    VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR"                    << std::endl;
    if (flags &                    VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR"                    << std::endl;
    if (flags &            VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT) std::clog << "\t\t\t - IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT"            << std::endl;
    if (flags &                  VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI) std::clog << "\t\t\t - IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI"                  << std::endl;
    if (flags &                      VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM) std::clog << "\t\t\t - IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM"                      << std::endl;
    if (flags &                 VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM) std::clog << "\t\t\t - IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM"                 << std::endl;
    if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_QUANTIZATION_DELTA_MAP_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_ENCODE_QUANTIZATION_DELTA_MAP_BIT_KHR" << std::endl;
    if (flags &           VK_IMAGE_USAGE_VIDEO_ENCODE_EMPHASIS_MAP_BIT_KHR) std::clog << "\t\t\t - IMAGE_USAGE_VIDEO_ENCODE_EMPHASIS_MAP_BIT_KHR"           << std::endl;
    if (flags &                   VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV) std::clog << "\t\t\t - IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV"                   << std::endl;
    if (flags &                       VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT) std::clog << "\t\t\t - IMAGE_USAGE_HOST_TRANSFER_BIT_EXT"                       << std::endl;
}

void DisplayColorSpaceKHR(VkColorSpaceKHR colorSpace) {
    std::clog << "\t\t\t - Color space     ";
    switch (colorSpace) {
        case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: {
			std::clog << "COLOR_SPACE_SRGB_NONLINEAR_KHR";
			break;
		}

        case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT: {
			std::clog << "COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT: {
			std::clog << "COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT: {
			std::clog << "COLOR_SPACE_DISPLAY_P3_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT: {
			std::clog << "COLOR_SPACE_DCI_P3_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_BT709_LINEAR_EXT: {
			std::clog << "COLOR_SPACE_BT709_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_BT709_NONLINEAR_EXT: {
			std::clog << "COLOR_SPACE_BT709_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_BT2020_LINEAR_EXT: {
			std::clog << "COLOR_SPACE_BT2020_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_HDR10_ST2084_EXT: {
			std::clog << "COLOR_SPACE_HDR10_ST2084_EXT";
			break;
		}

        case VK_COLOR_SPACE_DOLBYVISION_EXT: {
			std::clog << "COLOR_SPACE_DOLBYVISION_EXT";
			break;
		}

        case VK_COLOR_SPACE_HDR10_HLG_EXT: {
			std::clog << "COLOR_SPACE_HDR10_HLG_EXT";
			break;
		}

        case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT: {
			std::clog << "COLOR_SPACE_ADOBERGB_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT: {
			std::clog << "COLOR_SPACE_ADOBERGB_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_PASS_THROUGH_EXT: {
			std::clog << "COLOR_SPACE_PASS_THROUGH_EXT";
			break;
		}

        case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: {
			std::clog << "COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD: {
			std::clog << "COLOR_SPACE_DISPLAY_NATIVE_AMD";
			break;
		}

        default: {
            std::clog << "Unknown (" << colorSpace << ")";
            break;
        }
    }
    std::clog << std::endl;
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

void DisplayPhysicalDeviceLimits(VkPhysicalDeviceProperties const& properties) {
    std::clog << std::boolalpha;
    std::clog << "\t\tBuffer image granularity                                 "
              << properties.limits.bufferImageGranularity << std::endl;
    std::clog << "\t\tDiscrete queue priorities                                "
              << properties.limits.discreteQueuePriorities << std::endl;
    std::clog << "\t\tFramebuffer color sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.framebufferColorSampleCounts);
    std::clog << "\t\tFramebuffer depth sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.framebufferDepthSampleCounts);
    std::clog << "\t\tFramebuffer no attachments sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.framebufferNoAttachmentsSampleCounts);
    std::clog << "\t\tFramebuffer stencil sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.framebufferStencilSampleCounts);
    std::clog << "\t\tLine width granularity                                   "
              << properties.limits.lineWidthGranularity << std::endl;
    std::clog << "\t\tLine width min range                                     "
              << properties.limits.lineWidthRange[0] << std::endl;
    std::clog << "\t\tLine width max range                                     "
              << properties.limits.lineWidthRange[1] << std::endl;
    std::clog << "\t\tMax bound descriptor sets                                "
              << properties.limits.maxBoundDescriptorSets << std::endl;
    std::clog << "\t\tMax clip distances                                       "
              << properties.limits.maxClipDistances << std::endl;
    std::clog << "\t\tMax color attachments                                    "
              << properties.limits.maxColorAttachments << std::endl;
    std::clog << "\t\tMax combined clip and cull distances                     "
              << properties.limits.maxCombinedClipAndCullDistances << std::endl;
    std::clog << "\t\tMax compute shared memory size                           "
              << properties.limits.maxComputeSharedMemorySize << std::endl;
    std::clog << "\t\tMax compute work group count X                           "
              << properties.limits.maxComputeWorkGroupCount[0] << std::endl;
    std::clog << "\t\tMax compute work group count Y                           "
              << properties.limits.maxComputeWorkGroupCount[1] << std::endl;
    std::clog << "\t\tMax compute work group count Z                           "
              << properties.limits.maxComputeWorkGroupCount[2] << std::endl;
    std::clog << "\t\tMax compute work group invocations                       "
              << properties.limits.maxComputeWorkGroupInvocations << std::endl;
    std::clog << "\t\tMax compute work group size X                            "
              << properties.limits.maxComputeWorkGroupSize[0] << std::endl;
    std::clog << "\t\tMax compute work group size Y                            "
              << properties.limits.maxComputeWorkGroupSize[1] << std::endl;
    std::clog << "\t\tMax compute work group size Z                            "
              << properties.limits.maxComputeWorkGroupSize[2] << std::endl;
    std::clog << "\t\tMax cull distances                                       "
              << properties.limits.maxCullDistances << std::endl;
    std::clog << "\t\tMax descriptor set input attachments                     "
              << properties.limits.maxDescriptorSetInputAttachments << std::endl;
    std::clog << "\t\tMax descriptor set sampled images                        "
              << properties.limits.maxDescriptorSetSampledImages << std::endl;
    std::clog << "\t\tMax descriptor set samplers                              "
              << properties.limits.maxDescriptorSetSamplers << std::endl;
    std::clog << "\t\tMax descriptor set storage buffers                       "
              << properties.limits.maxDescriptorSetStorageBuffers << std::endl;
    std::clog << "\t\tMax descriptor set storage buffers dynamic               "
              << properties.limits.maxDescriptorSetStorageBuffersDynamic << std::endl;
    std::clog << "\t\tMax descriptor set storage images                        "
              << properties.limits.maxDescriptorSetStorageImages << std::endl;
    std::clog << "\t\tMax descriptor set uniform buffers                       "
              << properties.limits.maxDescriptorSetUniformBuffers << std::endl;
    std::clog << "\t\tMax descriptor set uniform buffers dynamic               "
              << properties.limits.maxDescriptorSetUniformBuffersDynamic << std::endl;
    std::clog << "\t\tMax draw indexed index value                             "
              << properties.limits.maxDrawIndexedIndexValue << std::endl;
    std::clog << "\t\tMax draw indirect count                                  "
              << properties.limits.maxDrawIndirectCount << std::endl;
    std::clog << "\t\tMax fragment combined output resources                   "
              << properties.limits.maxFragmentCombinedOutputResources << std::endl;
    std::clog << "\t\tMax fragment dual source attachments                     "
              << properties.limits.maxFragmentDualSrcAttachments << std::endl;
    std::clog << "\t\tMax fragment input components                            "
              << properties.limits.maxFragmentInputComponents << std::endl;
    std::clog << "\t\tMax fragment output components                           "
              << properties.limits.maxFragmentOutputAttachments << std::endl;
    std::clog << "\t\tMax framebuffer height                                   "
              << properties.limits.maxFramebufferHeight << std::endl;
    std::clog << "\t\tMax framebuffer layers                                   "
              << properties.limits.maxFramebufferLayers << std::endl;
    std::clog << "\t\tMax framebuffer width                                    "
              << properties.limits.maxFramebufferWidth << std::endl;
    std::clog << "\t\tMax geometry input components                            "
              << properties.limits.maxGeometryInputComponents << std::endl;
    std::clog << "\t\tMax geometry output components                           "
              << properties.limits.maxGeometryOutputComponents << std::endl;
    std::clog << "\t\tMax geometry output vertices                             "
              << properties.limits.maxGeometryOutputVertices << std::endl;
    std::clog << "\t\tMax geometry shader invocations                          "
              << properties.limits.maxGeometryShaderInvocations << std::endl;
    std::clog << "\t\tMax geometry total output components                     "
              << properties.limits.maxGeometryTotalOutputComponents << std::endl;
    std::clog << "\t\tMax image array layers                                   "
              << properties.limits.maxImageArrayLayers << std::endl;
    std::clog << "\t\tMax image dimension 1D                                   "
              << properties.limits.maxImageDimension1D << std::endl;
    std::clog << "\t\tMax image dimension 2D                                   "
              << properties.limits.maxImageDimension2D << std::endl;
    std::clog << "\t\tMax image dimension 3D                                   "
              << properties.limits.maxImageDimension3D << std::endl;
    std::clog << "\t\tMax image dimension cube                                 "
              << properties.limits.maxImageDimensionCube << std::endl;
    std::clog << "\t\tMax interpolation offset                                 "
              << properties.limits.maxInterpolationOffset << std::endl;
    std::clog << "\t\tMax memory allocation count                              "
              << properties.limits.maxMemoryAllocationCount << std::endl;
    std::clog << "\t\tMax per stage descriptor input attachments               "
              << properties.limits.maxPerStageDescriptorInputAttachments << std::endl;
    std::clog << "\t\tMax per stage descriptor sampled images                  "
              << properties.limits.maxPerStageDescriptorSampledImages << std::endl;
    std::clog << "\t\tMax per stage descriptor samplers                        "
              << properties.limits.maxPerStageDescriptorSamplers << std::endl;
    std::clog << "\t\tMax per stage descriptor storage buffers                 "
              << properties.limits.maxPerStageDescriptorStorageBuffers << std::endl;
    std::clog << "\t\tMax per stage descriptor storage images                  "
              << properties.limits.maxPerStageDescriptorStorageImages << std::endl;
    std::clog << "\t\tMax per stage descriptor uniform buffers                 "
              << properties.limits.maxPerStageDescriptorUniformBuffers << std::endl;
    std::clog << "\t\tMax per stage resources                                  "
              << properties.limits.maxPerStageResources << std::endl;
    std::clog << "\t\tMax push constants size                                  "
              << properties.limits.maxPushConstantsSize << std::endl;
    std::clog << "\t\tMax sample mask words                                    "
              << properties.limits.maxSampleMaskWords << std::endl;
    std::clog << "\t\tMax sampler allocation count                             "
              << properties.limits.maxSamplerAllocationCount << std::endl;
    std::clog << "\t\tMax sampler anisotropy                                   "
              << properties.limits.maxSamplerAnisotropy << std::endl;
    std::clog << "\t\tMax sampler LOD bias                                     "
              << properties.limits.maxSamplerLodBias << std::endl;
    std::clog << "\t\tMax storage buffer range                                 "
              << properties.limits.maxStorageBufferRange << std::endl;
    std::clog << "\t\tMax tesselation control per patch output components      "
              << properties.limits.maxTessellationControlPerPatchOutputComponents << std::endl;
    std::clog << "\t\tMax tesselation control per vertex input components      "
              << properties.limits.maxTessellationControlPerVertexInputComponents << std::endl;
    std::clog << "\t\tMax tesselation control per vertex output components     "
              << properties.limits.maxTessellationControlPerVertexOutputComponents << std::endl;
    std::clog << "\t\tMax tesselation control per total output components      "
              << properties.limits.maxTessellationControlTotalOutputComponents << std::endl;
    std::clog << "\t\tMax tesselation evaluation input components              "
              << properties.limits.maxTessellationEvaluationInputComponents << std::endl;
    std::clog << "\t\tMax tesselation evaluation output components             "
              << properties.limits.maxTessellationEvaluationOutputComponents << std::endl;
    std::clog << "\t\tMax tesselation generation level                         "
              << properties.limits.maxTessellationGenerationLevel << std::endl;
    std::clog << "\t\tMax tesselation generation patch size                    "
              << properties.limits.maxTessellationPatchSize << std::endl;
    std::clog << "\t\tMax texel buffer elements                                "
              << properties.limits.maxTexelBufferElements << std::endl;
    std::clog << "\t\tMax texel gather offset                                  "
              << properties.limits.maxTexelGatherOffset << std::endl;
    std::clog << "\t\tMax texel offset                                         "
              << properties.limits.maxTexelOffset << std::endl;
    std::clog << "\t\tMax uniform buffer range                                 "
              << properties.limits.maxUniformBufferRange << std::endl;
    std::clog << "\t\tMax vertex input attribute offset                        "
              << properties.limits.maxVertexInputAttributeOffset << std::endl;
    std::clog << "\t\tMax vertex input attributes                              "
              << properties.limits.maxVertexInputAttributes << std::endl;
    std::clog << "\t\tMax vertex input bindings                                "
              << properties.limits.maxVertexInputBindings << std::endl;
    std::clog << "\t\tMax vertex input binding stride                          "
              << properties.limits.maxVertexInputBindingStride << std::endl;
    std::clog << "\t\tMax vertex output components                             "
              << properties.limits.maxVertexOutputComponents << std::endl;
    std::clog << "\t\tMax viewport dimension X                                 "
              << properties.limits.maxViewportDimensions[0] << std::endl;
    std::clog << "\t\tMax viewport dimension Y                                 "
              << properties.limits.maxViewportDimensions[1] << std::endl;
    std::clog << "\t\tMax viewports                                            "
              << properties.limits.maxViewports << std::endl;
    std::clog << "\t\tMin interpolation offset                                 "
              << properties.limits.minInterpolationOffset << std::endl;
    std::clog << "\t\tMin memory map alignment                                 "
              << properties.limits.minMemoryMapAlignment << std::endl;
    std::clog << "\t\tMin storage buffer offset alignment                      "
              << properties.limits.minStorageBufferOffsetAlignment << std::endl;
    std::clog << "\t\tMin texel buffer offset alignment                        "
              << properties.limits.minTexelBufferOffsetAlignment << std::endl;
    std::clog << "\t\tMin texel gather offset                                  "
              << properties.limits.minTexelGatherOffset << std::endl;
    std::clog << "\t\tMin texel offset                                         "
              << properties.limits.minTexelOffset << std::endl;
    std::clog << "\t\tMin uniform buffer offset alignment                      "
              << properties.limits.minUniformBufferOffsetAlignment << std::endl;
    std::clog << "\t\tMipmap precision bits                                    "
              << properties.limits.mipmapPrecisionBits << std::endl;
    std::clog << "\t\tNon coherent atom size                                   "
              << properties.limits.nonCoherentAtomSize << std::endl;
    std::clog << "\t\tOptimal buffer copy offset alignment                     "
              << properties.limits.optimalBufferCopyOffsetAlignment << std::endl;
    std::clog << "\t\tOptimal buffer copy row pitch alignment                  "
              << properties.limits.optimalBufferCopyRowPitchAlignment << std::endl;
    std::clog << "\t\tPoint size granularity                                   "
              << properties.limits.pointSizeGranularity << std::endl;
    std::clog << "\t\tPoint size min range                                     "
              << properties.limits.pointSizeRange[0] << std::endl;
    std::clog << "\t\tPoint size max range                                     "
              << properties.limits.pointSizeRange[1] << std::endl;
    std::clog << "\t\tSampled image color sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.sampledImageColorSampleCounts);
    std::clog << "\t\tSampled image depth sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.sampledImageDepthSampleCounts);
    std::clog << "\t\tSampled image integer sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.sampledImageIntegerSampleCounts);
    std::clog << "\t\tSampled image stencil sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.sampledImageStencilSampleCounts);
    std::clog << "\t\tSparse address space size                                "
              << properties.limits.sparseAddressSpaceSize << std::endl;
    std::clog << "\t\tStandard sample locations                                "
              << static_cast<bool>(properties.limits.standardSampleLocations) << std::endl;
    std::clog << "\t\tStandard image sample counts:" << std::endl;
    DisplaySampleCountFlags(properties.limits.storageImageSampleCounts);
    std::clog << "\t\tStrict lines                                             "
              << static_cast<bool>(properties.limits.strictLines) << std::endl;
    std::clog << "\t\tSub pixel interpolation offset bits                      "
              << properties.limits.subPixelInterpolationOffsetBits << std::endl;
    std::clog << "\t\tSub pixel precision bits                                 "
              << properties.limits.subPixelPrecisionBits << std::endl;
    std::clog << "\t\tSub texel precision bits                                 "
              << properties.limits.subTexelPrecisionBits << std::endl;
    std::clog << "\t\tTimestamp compute and graphics                           "
              << static_cast<bool>(properties.limits.timestampComputeAndGraphics) << std::endl;
    std::clog << "\t\tTimestamp period                                         "
              << properties.limits.timestampPeriod << std::endl;
    std::clog << "\t\tViewport bounds min range                                "
              << properties.limits.viewportBoundsRange[0] << std::endl;
    std::clog << "\t\tViewport bounds max range                                "
              << properties.limits.viewportBoundsRange[1] << std::endl;
    std::clog << "\t\tViewport sub pixel bits                                  "
              << properties.limits.viewportSubPixelBits << std::endl;
    std::clog << std::noboolalpha;
}

void DisplayPhysicalDeviceSparseProperties(VkPhysicalDeviceSparseProperties const& sparseProperties) {
    std::clog << std::boolalpha;
    std::clog << "\t\t - Residency aligned mip size                        "
              << static_cast<bool>(sparseProperties.residencyAlignedMipSize) << std::endl;
    std::clog << "\t\t - Residency non resident strict                     "
              << static_cast<bool>(sparseProperties.residencyNonResidentStrict) << std::endl;
    std::clog << "\t\t - Residency standard 2D block shape                 "
              << static_cast<bool>(sparseProperties.residencyStandard2DBlockShape) << std::endl;
    std::clog << "\t\t - Residency standard 2D multisample block shape     "
              << static_cast<bool>(sparseProperties.residencyStandard2DMultisampleBlockShape) << std::endl;
    std::clog << "\t\t - Residency standard 3D block shape                 "
              << static_cast<bool>(sparseProperties.residencyStandard3DBlockShape) << std::endl;
    std::clog << std::noboolalpha;
}

void DisplayPhysicalDeviceQueueFamilyProperties(VkQueueFamilyProperties const& queueFamilyProperties) {
    std::clog << "\t\t\t - Min image transfer granularity width:" << std::endl;
    std::clog << "\t\t\t\t - Width      "
              << queueFamilyProperties.minImageTransferGranularity.width << std::endl;
    std::clog << "\t\t\t\t - Height     "
              << queueFamilyProperties.minImageTransferGranularity.height << std::endl;
    std::clog << "\t\t\t\t - Depth      "
              << queueFamilyProperties.minImageTransferGranularity.depth << std::endl;
    std::clog << "\t\t\t - Queue count              "
              << queueFamilyProperties.queueCount << std::endl;
    std::clog << "\t\t\t - Queue flags:" << std::endl;
    DisplayQueueFlags(queueFamilyProperties.queueFlags);
    std::clog << "\t\t\t - Timestamp valid bits     "
              << queueFamilyProperties.timestampValidBits << std::endl;
}

void DisplaySupportedExtensionsFromSDLVulkan(std::vector<const char*> const& enabledExtensions, std::vector<VkExtensionProperties> const& supportedExtensions) {
    uint32_t supportedEnabledExtensionsCount = 0;
    for (char const* enabledExtension : enabledExtensions) {
        for (VkExtensionProperties const& supportedExtension : supportedExtensions) {
            if (strcmp(enabledExtension, supportedExtension.extensionName) == 0) {
                std::cout << enabledExtension << " is supported" << std::endl;
                ++supportedEnabledExtensionsCount;
            }
        }
    }

    if (enabledExtensions.size() == supportedEnabledExtensionsCount) {
        std::cout << "All enabled extensions are supported" << std::endl;
    }
}

void DisplayPhysicalDeviceProperties(VkPhysicalDeviceProperties2 const& physicalDeviceProperties, bool skipLimits = true, bool skipSparseProperties = true) {
    Version apiVersion(physicalDeviceProperties.properties.apiVersion);
    std::clog << "\t - API version: " << apiVersion << std::endl; 
    std::clog << "\t - Device ID: " << physicalDeviceProperties.properties.deviceID << std::endl; 
    std::clog << "\t - Device name: " << physicalDeviceProperties.properties.deviceName << std::endl; 

    std::clog << "\t - Device type: ";
    switch (physicalDeviceProperties.properties.deviceType) {
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU: {
            std::clog << "CPU";
            break;
        }

        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
            std::clog << "Discrete GPU";
            break;
        }

        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
            std::clog << "Integrated GPU";
            break;
        }

        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER: {
            std::clog << "Other";
            break;
        }
        
        case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: {
            std::clog << "Virtual GPU";
            break;
        }

        default: {
            std::clog << "Unknown";
            break;
        }
    }
    std::clog << std::endl;

    DriverVersion driverVersion(
          physicalDeviceProperties.properties.driverVersion,
          physicalDeviceProperties.properties.vendorID
    );
    std::clog << "\t - Driver version: " << driverVersion << std::endl; 
    std::clog << "\t - Limits:" << std::endl;
    if (skipLimits) {
        std::clog << "\t\t(skipped)" << std::endl;
    }

    else {
        DisplayPhysicalDeviceLimits(physicalDeviceProperties.properties);
    }

    std::clog << std::hex << "\t - Pipeline cache UUID: ";
    for (int i = 0; i < 16; ++i) {
        std::clog << std::setw(2) << std::setfill('0') << static_cast<int>(physicalDeviceProperties.properties.pipelineCacheUUID[i]) << (i != 15 ? ":" : "");
    }
    std::clog << std::dec << std::endl;;
    
    std::clog << "\t - Sparse properties:" << std::endl;
    if (skipSparseProperties) {
        std::clog << "\t\t(skipped)" << std::endl;
    }
    
    else {
        DisplayPhysicalDeviceSparseProperties(physicalDeviceProperties.properties.sparseProperties);
    }

    std::clog << "\t - Vendor ID: " << physicalDeviceProperties.properties.vendorID << std::endl;
}

void DisplayPhysicalDeviceFeatures(VkPhysicalDeviceFeatures2 const& physicalDeviceFeatures) {
    std::clog << std::boolalpha;
    std::clog << "\t - Features:" << std::endl;
    std::clog << "\t\t - Alpha to one                                     " 
              << static_cast<bool>(physicalDeviceFeatures.features.alphaToOne) << std::endl;
    std::clog << "\t\t - Depth bias clamp                                 " 
              << static_cast<bool>(physicalDeviceFeatures.features.depthBiasClamp) << std::endl;
    std::clog << "\t\t - Depth bounds                                     " 
              << static_cast<bool>(physicalDeviceFeatures.features.depthBounds) << std::endl;
    std::clog << "\t\t - Depth clamp                                      " 
              << static_cast<bool>(physicalDeviceFeatures.features.depthClamp) << std::endl;
    std::clog << "\t\t - Draw indirect first instance                     " 
              << static_cast<bool>(physicalDeviceFeatures.features.drawIndirectFirstInstance) << std::endl;
    std::clog << "\t\t - Dual source blend                                " 
              << static_cast<bool>(physicalDeviceFeatures.features.dualSrcBlend) << std::endl;
    std::clog << "\t\t - Fill mode non solid                              " 
              << static_cast<bool>(physicalDeviceFeatures.features.fillModeNonSolid) << std::endl;
    std::clog << "\t\t - Fragment stores and atomics                      " 
              << static_cast<bool>(physicalDeviceFeatures.features.fragmentStoresAndAtomics) << std::endl;
    std::clog << "\t\t - Full draw index Uint32                           " 
              << static_cast<bool>(physicalDeviceFeatures.features.fullDrawIndexUint32) << std::endl;
    std::clog << "\t\t - Geometry shader                                  " 
              << static_cast<bool>(physicalDeviceFeatures.features.geometryShader) << std::endl;
    std::clog << "\t\t - Image cube array                                 " 
              << static_cast<bool>(physicalDeviceFeatures.features.imageCubeArray) << std::endl;
    std::clog << "\t\t - Independent blend                                " 
              << static_cast<bool>(physicalDeviceFeatures.features.independentBlend) << std::endl;
    std::clog << "\t\t - Inherited queries                                " 
              << static_cast<bool>(physicalDeviceFeatures.features.inheritedQueries) << std::endl;
    std::clog << "\t\t - Large points                                     " 
              << static_cast<bool>(physicalDeviceFeatures.features.largePoints) << std::endl;
    std::clog << "\t\t - Logic operations                                 " 
              << static_cast<bool>(physicalDeviceFeatures.features.logicOp) << std::endl;
    std::clog << "\t\t - Multi draw indirect                              " 
              << static_cast<bool>(physicalDeviceFeatures.features.multiDrawIndirect) << std::endl;
    std::clog << "\t\t - Multi viewport                                   " 
              << static_cast<bool>(physicalDeviceFeatures.features.multiViewport) << std::endl;
    std::clog << "\t\t - Occlusion query precise                          " 
              << static_cast<bool>(physicalDeviceFeatures.features.occlusionQueryPrecise) << std::endl;
    std::clog << "\t\t - Pipeline statistics query                        " 
              << static_cast<bool>(physicalDeviceFeatures.features.pipelineStatisticsQuery) << std::endl;
    std::clog << "\t\t - Robust buffer access                             " 
              << static_cast<bool>(physicalDeviceFeatures.features.robustBufferAccess) << std::endl;
    std::clog << "\t\t - Sampler anisotropy                               " 
              << static_cast<bool>(physicalDeviceFeatures.features.samplerAnisotropy) << std::endl;
    std::clog << "\t\t - Sample rate shading                              " 
              << static_cast<bool>(physicalDeviceFeatures.features.sampleRateShading) << std::endl;
    std::clog << "\t\t - Shader clip distance                             " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderClipDistance) << std::endl;
    std::clog << "\t\t - Shader cull distance                             " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderCullDistance) << std::endl;
    std::clog << "\t\t - Shader Float64                                   " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderFloat64) << std::endl;
    std::clog << "\t\t - Shader image gather extended                     " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderImageGatherExtended) << std::endl;
    std::clog << "\t\t - Shader Int16                                     " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderInt16) << std::endl;
    std::clog << "\t\t - Shader Int64                                     "  
              << static_cast<bool>(physicalDeviceFeatures.features.shaderInt64) << std::endl;
    std::clog << "\t\t - Shader resource min LOD                          " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderResourceMinLod) << std::endl;
    std::clog << "\t\t - Shader resource residency                        " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderResourceResidency) << std::endl;
    std::clog << "\t\t - Shader sampled image array dynamic indexing      " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderSampledImageArrayDynamicIndexing) << std::endl;
    std::clog << "\t\t - Shader storage buffer array dynamic indexing     " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageBufferArrayDynamicIndexing) << std::endl;
    std::clog << "\t\t - Shader storage image array dynamic indexing      " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageArrayDynamicIndexing) << std::endl;
    std::clog << "\t\t - Shader storage image extended formats            " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageExtendedFormats) << std::endl;
    std::clog << "\t\t - Shader storage image multisample                 " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageMultisample) << std::endl;
    std::clog << "\t\t - Shader storage image read without format         " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageReadWithoutFormat) << std::endl;
    std::clog << "\t\t - Shader storage image write withou format         " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageWriteWithoutFormat) << std::endl;
    std::clog << "\t\t - Shader tesselation and geometry point size       " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderTessellationAndGeometryPointSize) << std::endl;
    std::clog << "\t\t - Shader uniform buffer array dynamic indexing     " 
              << static_cast<bool>(physicalDeviceFeatures.features.shaderUniformBufferArrayDynamicIndexing) << std::endl;
    std::clog << "\t\t - Sparse binding                                   " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseBinding) << std::endl;
    std::clog << "\t\t - Sparse residency 16 samples                      " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency16Samples) << std::endl;
    std::clog << "\t\t - Sparse residency 2 samples                       " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency2Samples) << std::endl;
    std::clog << "\t\t - Sparse residency 4 samples                       " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency4Samples) << std::endl;
    std::clog << "\t\t - Sparse residency 8 samples                       " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency8Samples) << std::endl;
    std::clog << "\t\t - Sparse residency aliased                         " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyAliased) << std::endl;
    std::clog << "\t\t - Sparse residency buffer                          " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyBuffer) << std::endl;
    std::clog << "\t\t - Sparse residency image 2D                        " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyImage2D) << std::endl;
    std::clog << "\t\t - Sparse residency image 3D                        " 
              << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyImage3D) << std::endl;
    std::clog << "\t\t - Tesselation shader                               " 
              << static_cast<bool>(physicalDeviceFeatures.features.tessellationShader) << std::endl;
    std::clog << "\t\t - Texture compression ASTC_LDR                     " 
              << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionASTC_LDR) << std::endl;
    std::clog << "\t\t - Texture compression BC                           " 
              << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionBC) << std::endl;
    std::clog << "\t\t - Texture compression ETC2                         " 
              << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionETC2) << std::endl;
    std::clog << "\t\t - Variable multisample rate                        " 
              << static_cast<bool>(physicalDeviceFeatures.features.variableMultisampleRate) << std::endl;
    std::clog << "\t\t - Vertex pipeline stores and atomics               " 
              << static_cast<bool>(physicalDeviceFeatures.features.vertexPipelineStoresAndAtomics) << std::endl;
    std::clog << "\t\t - Wide lines                                       " 
              << static_cast<bool>(physicalDeviceFeatures.features.wideLines) << std::endl;
    std::clog << std::noboolalpha;
}

void DisplayInstanceLayerProperties(VkLayerProperties const& instanceLayerProperties) {
    std::cout << "\tInstance layer properties found:" << std::endl;
    std::cout << "\t\t - Description                "
              << instanceLayerProperties.description << std::endl;
    std::cout << "\t\t - Implementation version     " 
              << instanceLayerProperties.implementationVersion << std::endl;
    std::cout << "\t\t - Name                       "
              << instanceLayerProperties.layerName << std::endl;
    Version specVersion(instanceLayerProperties.specVersion);
    std::cout << "\t\t - Specification version      "
              << specVersion << std::endl;
}

void DisplayInstanceExtensionProperties(VkExtensionProperties const& instanceExtensionProperties) {
    std::clog << "\tInstance extension found:" << std::endl;
    std::clog << "\t\t - Name:                      "
              << instanceExtensionProperties.extensionName << std::endl; 
    std::clog << "\t\t - Specification version:     "
              << instanceExtensionProperties.specVersion << std::endl; 
}

void DisplayPhysicalDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR const& physicalDeviceSurfaceCapabilities) {
    std::clog << "\t - Surface capabilities" << std::endl;
    std::clog << "\t\t - Current extent height         " 
              << physicalDeviceSurfaceCapabilities.currentExtent.height << std::endl;
    std::clog << "\t\t - Current extent width          "
              << physicalDeviceSurfaceCapabilities.currentExtent.width << std::endl;
    std::clog << "\t\t - Current transforms:" << std::endl;
    DisplaySurfaceTransformFlagsKHR(physicalDeviceSurfaceCapabilities.currentTransform);
    std::clog << "\t\t - Max image array layers        "
              << physicalDeviceSurfaceCapabilities.maxImageArrayLayers << std::endl;
    std::clog << "\t\t - Max image count               "
              << physicalDeviceSurfaceCapabilities.maxImageCount << std::endl;
    std::clog << "\t\t - Max image extent height       "
              << physicalDeviceSurfaceCapabilities.maxImageExtent.height << std::endl;
    std::clog << "\t\t - Max image extent width        "
              << physicalDeviceSurfaceCapabilities.maxImageExtent.width << std::endl;
    std::clog << "\t\t - Min image count               "
              << physicalDeviceSurfaceCapabilities.minImageCount << std::endl;
    std::clog << "\t\t - Supported composite alpha     "
              << physicalDeviceSurfaceCapabilities.supportedCompositeAlpha << std::endl;
    std::clog << "\t\t - Supported transforms:" << std::endl;
    DisplaySurfaceTransformFlagsKHR(physicalDeviceSurfaceCapabilities.supportedTransforms);
    std::clog << "\t\t - Supported usage flags:" << std::endl;
    DisplayImageUsageFlags(physicalDeviceSurfaceCapabilities.supportedUsageFlags);
}

void DisplaySurfaceFormat(VkSurfaceFormatKHR const& surfaceFormat) {
    DisplayColorSpaceKHR(surfaceFormat.colorSpace);
    std::clog << "\t\t\t - Format          "
              << surfaceFormat.format << std::endl;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* pUserData
) {
    (void)(pUserData);

    std::clog << "[ ";
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            std::clog << "VERBOSE";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            std::clog << "INFO";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            std::clog << "WARNING";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            std::clog << "ERROR";
            break;
        }

        default: {
            break;
        }
    }

    std::clog << "::";
    
    switch (messageType) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
            std::clog << "GENERAL"; 
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
            std::clog << "VALIDATION";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
            std::clog << "PERFORMANCE";
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: {
            std::clog << "DEVICE ADRESS BINDING";
            break; 
        }

        default: {
            break;
        }
    }

    std::cout << " ] ";

    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

int main(int argc, char* argv[]) {
    (void)(argc);
    (void)(argv);

    std::clog << "Vulkan header version: " << VK_HEADER_VERSION << std::endl;

    SDL_Window* window = nullptr;
    SDL_Event event {};
    bool running = false;
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    std::optional<uint32_t> queueFamilyIndexWithGraphicsCapabilities = std::nullopt;
    std::optional<uint32_t> queueFamilyIndexWithPresentCapabilities = std::nullopt;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImageView> swapChainImageViews {};
    VkShaderModule vertexShaderModule = VK_NULL_HANDLE;
    VkShaderModule fragmentShaderModule = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> frameBuffers {};
    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
    VkFence inFlightFence = VK_NULL_HANDLE;
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT debugUtilsMessenger = VK_NULL_HANDLE;
    constexpr bool enableValidationLayers = true;
#else
    constexpr bool enableValidationLayers = false;
#endif

    auto CleanOnExit = [&](int code) {
        if (imageAvailableSemaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, imageAvailableSemaphore, VK_NULL_HANDLE);
            std::clog << "'Image available' semaphore destroyed successfully" << std::endl;
            imageAvailableSemaphore = VK_NULL_HANDLE;
        }

        if (renderFinishedSemaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, renderFinishedSemaphore, VK_NULL_HANDLE);
            std::clog << "'Render finished' semaphore destroyed successfully" << std::endl;
            renderFinishedSemaphore = VK_NULL_HANDLE;
        }

        if (inFlightFence != VK_NULL_HANDLE) {
            vkDestroyFence(device, inFlightFence, VK_NULL_HANDLE);
            std::clog << "'In flight' fence destroyed successfully" << std::endl;
            inFlightFence = VK_NULL_HANDLE;
        }

        if (commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device, commandPool, VK_NULL_HANDLE);
            std::clog << "Command pool destroyed successfully" << std::endl;
            commandPool = VK_NULL_HANDLE;
        }

        for (VkFramebuffer& frameBuffer : frameBuffers) {
            if (frameBuffer != VK_NULL_HANDLE) {
                vkDestroyFramebuffer(device, frameBuffer, VK_NULL_HANDLE);
                std::clog << "Framebuffer destroyed successfully" << std::endl;
                frameBuffer = VK_NULL_HANDLE;
            }
        }

        if (graphicsPipeline != VK_NULL_HANDLE) {
            vkDestroyPipeline(device, graphicsPipeline, VK_NULL_HANDLE);
            std::clog << "Graphics pipeline destroyed successfully" << std::endl;
            graphicsPipeline = VK_NULL_HANDLE;
        }

        if (pipelineLayout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(device, pipelineLayout, VK_NULL_HANDLE);
            std::clog << "Pipeline layout destroyed successfully" << std::endl;
            pipelineLayout = VK_NULL_HANDLE;
        }

        if (renderPass != VK_NULL_HANDLE) {
            vkDestroyRenderPass(device, renderPass, VK_NULL_HANDLE);
            std::clog << "Render pass destroyed successfully" << std::endl;
            renderPass = VK_NULL_HANDLE;
        }

        if (fragmentShaderModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(device, fragmentShaderModule, VK_NULL_HANDLE);
            std::clog << "Fragment shader module destroyed successfully" << std::endl;
            fragmentShaderModule = VK_NULL_HANDLE;
        }

        if (vertexShaderModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(device, vertexShaderModule, VK_NULL_HANDLE);
                std::clog << "Vertex shader module destroyed successfully" << std::endl;
                vertexShaderModule = VK_NULL_HANDLE;
        }

        for (VkImageView& swapChainImageView : swapChainImageViews) {
            if (swapChainImageView != VK_NULL_HANDLE) {
                // destroy image view
                vkDestroyImageView(device, swapChainImageView, VK_NULL_HANDLE);
                std::clog << "Swap chain image view destroyed successfully" << std::endl;
                swapChainImageView = VK_NULL_HANDLE;
            }
        }

        if (swapChain != VK_NULL_HANDLE) {
            // destroy swapchain
            vkDestroySwapchainKHR(device, swapChain, VK_NULL_HANDLE);
            std::clog << "Swap chain destroyed successfully" << std::endl;
            swapChain = VK_NULL_HANDLE;
        }

        if (surface != VK_NULL_HANDLE) {
            // destroy surface
            vkDestroySurfaceKHR(instance, surface, VK_NULL_HANDLE);
            std::clog << "Surface destroyed successfully" << std::endl;
            surface = VK_NULL_HANDLE;
        }

        if (device != VK_NULL_HANDLE) {
            // destroy device and associated queues
            vkDestroyDevice(device, VK_NULL_HANDLE);
            std::clog << "Device destroyed successfully" << std::endl;
            device = VK_NULL_HANDLE;
        }

#ifndef NDEBUG
        if (enableValidationLayers && debugUtilsMessenger != VK_NULL_HANDLE) {
            // destroy debug messenger
            auto DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
            if (DestroyDebugUtilsMessengerEXT == VK_NULL_HANDLE) {
                std::cerr << "Unable to get instance process address for vkCreateDebugUtilsMessengerEXT" << std::endl;
            }

            else {
                DestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, VK_NULL_HANDLE);
                std::clog << "Debug messenger destroyed successfully" << std::endl;
                debugUtilsMessenger = VK_NULL_HANDLE;
            }
        }
#endif

        if (instance != VK_NULL_HANDLE) {
            // destroy instance and enumerated physical devices
            vkDestroyInstance(instance, VK_NULL_HANDLE);
            std::clog << "Vulkan instance destroyed successfully" << std::endl;
            instance = VK_NULL_HANDLE;
        }

        if (window != nullptr) {
            // destroy window
            SDL_DestroyWindow(window);
            std::clog << "Window destroyed successfully" << std::endl;
            window = nullptr;
        }

        SDL_Quit();

        return code;
    };
    
    // init SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Unable to initialize the SDL (reason: " << SDL_GetError() << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "SDL initialized successfully" << std::endl;

    // create window 
    SDL_PropertiesID windowProperties = SDL_CreateProperties();
    SDL_SetStringProperty(windowProperties, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "SDL3 with Vulkan");
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 640);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 480);
    SDL_SetNumberProperty(windowProperties, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, SDL_WINDOW_VULKAN);
    window = SDL_CreateWindowWithProperties(windowProperties);
    if (window == nullptr) {
        std::cerr << "Unable to create a window (reason: " << SDL_GetError() << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Window created successfully: <SDL_Window " << window << ">" << std::endl; 
    SDL_DestroyProperties(windowProperties);

    // create application info
    VkApplicationInfo applicationInfo {};
    applicationInfo.apiVersion = VK_API_VERSION_1_4;
    applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    applicationInfo.pApplicationName = "SDL3 with Vulkan application";
    applicationInfo.pEngineName = "No engine";
    applicationInfo.pNext = VK_NULL_HANDLE;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    // validation layers to use
    std::vector<char const*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    // enumerate instance layers properties
    uint32_t instanceLayersPropertiesCount = 0;
    VkResult enumerateInstanceLayerPropertiesResult = vkEnumerateInstanceLayerProperties(&instanceLayersPropertiesCount, VK_NULL_HANDLE);
    if (enumerateInstanceLayerPropertiesResult != VK_SUCCESS) {
        std::cerr << "Unable to enumerate instance layer properties (1st call, status: " << enumerateInstanceLayerPropertiesResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Instance layer properties enumerated successfully (1st call, count: " << instanceLayersPropertiesCount << ")" << std::endl;
    
    std::vector<VkLayerProperties> instanceLayersProperties(instanceLayersPropertiesCount, VkLayerProperties {});
    enumerateInstanceLayerPropertiesResult = vkEnumerateInstanceLayerProperties(&instanceLayersPropertiesCount, instanceLayersProperties.data());
    if (enumerateInstanceLayerPropertiesResult != VK_SUCCESS) {
        std::cerr << "Unable to enumerate instance layer properties (2nd call, status: " << enumerateInstanceLayerPropertiesResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Instance layer properties enumerated successfully (2nd call, retrieved in array)" << std::endl;

    // display instance layers properties
    std::clog << "Instance layers properties:" << std::endl;
    for (VkLayerProperties const& instanceLayerProperties : instanceLayersProperties) {
        DisplayInstanceLayerProperties(instanceLayerProperties);
    }

    // check validation layers support and enable them if available
    bool validationLayersSupported = true;
    for (char const* layerName : validationLayers) {
        bool layerFound = false;

        for (VkLayerProperties const& instanceLayerProperties : instanceLayersProperties) {
            if (strcmp(layerName, instanceLayerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            validationLayersSupported = false;
            break;
        }
    }

    if (enableValidationLayers && !validationLayersSupported) {
        std::cerr << "Validation layers requested, but not available" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }

    // get enabled instance extensions
    Uint32 enabledExtensionsCount = 0;
    char const* const* enabledExtensionsArray = SDL_Vulkan_GetInstanceExtensions(&enabledExtensionsCount);
    std::vector<char const*> enabledExtensions {};
    for (int i = 0; i < static_cast<int>(enabledExtensionsCount); ++i) {
        enabledExtensions.emplace_back(enabledExtensionsArray[i]);
    }

    // for MacOS, to avoid VK_ERROR_INCOMPATIBLE_DRIVER on vkCreateInstance
    enabledExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    enabledExtensions.emplace_back("VK_KHR_get_surface_capabilities2");

    if (enableValidationLayers) {
        enabledExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    std::clog << "Enabled extensions count retrieved successfully from SDL_Vulkan_GetInstanceExtensions (count: " << enabledExtensions.size() << ")" << std::endl;
    std::clog << "Enabled extensions:" << std::endl;
    for (const char* enabledExtension : enabledExtensions) {
        std::clog << "\t - " << enabledExtension << std::endl;
    }

    // get instance extensions properties
    uint32_t instanceExtensionsPropertiesCount = 0;
    VkResult enumerateInstanceExtensionsPropertiesResult = vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &instanceExtensionsPropertiesCount, VK_NULL_HANDLE);
    if (enumerateInstanceExtensionsPropertiesResult != VK_SUCCESS) {
        std::cerr << "Unable to enumerate instance extensions properties (1st call, status: " << enumerateInstanceExtensionsPropertiesResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Instance extensions properties enumerated successfully (1st call, count: " << instanceExtensionsPropertiesCount << ")" << std::endl;

    std::vector<VkExtensionProperties> instanceExtensionsProperties(instanceExtensionsPropertiesCount, VkExtensionProperties {});
    enumerateInstanceExtensionsPropertiesResult = vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &instanceExtensionsPropertiesCount, instanceExtensionsProperties.data());
    if (enumerateInstanceExtensionsPropertiesResult != VK_SUCCESS) {
        std::cerr << "Unable to enumerate instance extensions properties (2nd call, status: " << enumerateInstanceExtensionsPropertiesResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Instance extensions properties enumerated successfully (2nd call, retrived in array)" << std::endl;

    std::clog << "Instance extensions:" << std::endl;
    for (VkExtensionProperties const& instanceExtensionProperties : instanceExtensionsProperties) {
        DisplayInstanceExtensionProperties(instanceExtensionProperties);
    }

    DisplaySupportedExtensionsFromSDLVulkan(enabledExtensions, instanceExtensionsProperties);

#ifndef NDEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo {};

    if (enableValidationLayers) {
        // create debug utils messenger for vkCreateInstance and vkDestroyInstance
        debugUtilsMessengerCreateInfo.flags = 0;
        debugUtilsMessengerCreateInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
            ;
        debugUtilsMessengerCreateInfo.messageType = 
        //    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
            ;
        debugUtilsMessengerCreateInfo.pfnUserCallback = DebugCallback;
        debugUtilsMessengerCreateInfo.pNext = VK_NULL_HANDLE;
        debugUtilsMessengerCreateInfo.pUserData = VK_NULL_HANDLE;
        debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    }
#endif

    // create instance
    VkInstanceCreateInfo instanceCreateInfo {};
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    instanceCreateInfo.enabledLayerCount = (enableValidationLayers ? static_cast<uint32_t>(validationLayers.size()) : 0);
    instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
#ifndef NDEBUG
    instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;
#else
    instanceCreateInfo.pNext = VK_NULL_HANDLE;
#endif
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    instanceCreateInfo.ppEnabledLayerNames = (enableValidationLayers ? validationLayers.data() : VK_NULL_HANDLE);
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    VkResult instanceCreateResult = vkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &instance);
    if (instanceCreateResult != VkResult::VK_SUCCESS) {
        std::cerr << "Unable to create a Vulkan instance (result: " << instanceCreateResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Vulkan instance created successfully: <VkInstance " << instance << ">" << std::endl;

#ifndef NDEBUG
    auto CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (CreateDebugUtilsMessengerEXT == VK_NULL_HANDLE) {
        std::cerr << "Unable to get instance process address for vkCreateDebugUtilsMessengerEXT" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }

    VkResult createDebugUtilsMessengerResult = CreateDebugUtilsMessengerEXT(instance, &debugUtilsMessengerCreateInfo, VK_NULL_HANDLE, &debugUtilsMessenger);
    if (createDebugUtilsMessengerResult != VK_SUCCESS) {
        std::cerr << "Unable to create a debug utils messenger (status: " << createDebugUtilsMessengerResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
#endif

    // get window properties for surface create info
    SDL_PropertiesID tmpWindowProperties = SDL_GetWindowProperties(window);
    HWND hwnd = (HWND)(SDL_GetPointerProperty(tmpWindowProperties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
    if (hwnd == nullptr) {
        std::cerr << "Unable to get the window's HWND pointer" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Window's HWND pointer retrieved successfully" << std::endl;
    
    HINSTANCE hinstance = (HINSTANCE)(SDL_GetPointerProperty(tmpWindowProperties, SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER, nullptr));
    if (hinstance == nullptr) {
        std::cerr << "Unable to get the window's HINSTANCE pointer" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Window's HINSTANCE pointer retrieved successfully" << std::endl;

    SDL_DestroyProperties(tmpWindowProperties);

    // create surface
    VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo {};
    win32SurfaceCreateInfo.flags = 0;
    win32SurfaceCreateInfo.hinstance = hinstance;
    win32SurfaceCreateInfo.hwnd = hwnd;
    win32SurfaceCreateInfo.pNext = VK_NULL_HANDLE;
    win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;

    VkResult createWin32SurfaceKHRResult = vkCreateWin32SurfaceKHR(instance, &win32SurfaceCreateInfo, VK_NULL_HANDLE, &surface);
    if (createWin32SurfaceKHRResult != VK_SUCCESS) {
        std::cerr << "Unable to create a surface (status: " << createWin32SurfaceKHRResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Surface created successfully: <VkSurface " << surface << ">" << std::endl;

    // get physical devices count
    uint32_t physicalDeviceCount = 0;
    VkResult enumeratePhysicalDeviceResult = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, VK_NULL_HANDLE);
    if (enumeratePhysicalDeviceResult != VkResult::VK_SUCCESS) {
        std::cerr << "Unable to enumerate physical devices (1st call, status: " << enumeratePhysicalDeviceResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Physical devices enumerated successfully (1st call, count: " << physicalDeviceCount << ")" << std::endl;

    // should not happen on a modern PC because the CPU can be used
    if (physicalDeviceCount == 0) {
        std::cerr << "No physical devices found" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }

    // enumerate physical devices
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount, VK_NULL_HANDLE);
    enumeratePhysicalDeviceResult = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
    if (enumeratePhysicalDeviceResult != VkResult::VK_SUCCESS) {
        std::cerr << "Unable to enumerate physical devices (2nd call, status: " << enumeratePhysicalDeviceResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Physical devices enumerated successfully (2nd call, retrieved in array)" << std::endl;

    // specify device extensions
    std::vector<char const*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkSurfaceFormat2KHR preferredFormat {};
    VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    VkExtent2D swapchainExtent {};
    uint32_t imageCount = 0;
    VkSurfaceTransformFlagBitsKHR currentTransform {};

    // enumerate each physical device's properties
    std::ios_base::fmtflags formatFlags = std::clog.flags();
    for (VkPhysicalDevice const& pd : physicalDevices) {
        std::clog << "Physical device found: <VkPhysicalDevice " << pd << ">" << std::endl;
        
        // get physical device's properties
        VkPhysicalDeviceProperties2 physicalDeviceProperties {};
        physicalDeviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vkGetPhysicalDeviceProperties2(pd, &physicalDeviceProperties);

        // get physical device's features
        VkPhysicalDeviceFeatures2 physicalDeviceFeatures {};
        physicalDeviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        vkGetPhysicalDeviceFeatures2(pd, &physicalDeviceFeatures);

        // get physical device's queue family properties
        uint32_t physicalDeviceQueueFamilyPropertiesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties2(pd, &physicalDeviceQueueFamilyPropertiesCount, VK_NULL_HANDLE);
        if (physicalDeviceQueueFamilyPropertiesCount == 0) {
            std::cerr << "Could not find any queue family properties" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }

        std::vector<VkQueueFamilyProperties2> physicalDeviceQueueFamilyProperties(
            physicalDeviceQueueFamilyPropertiesCount,
            VkQueueFamilyProperties2 { .sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2 }
        );
    
        vkGetPhysicalDeviceQueueFamilyProperties2(pd, &physicalDeviceQueueFamilyPropertiesCount, physicalDeviceQueueFamilyProperties.data());
        int queueFamilyIndex = 0;
        std::clog << "\t - Queue families (count: " << physicalDeviceQueueFamilyPropertiesCount << ")" << std::endl;
        for (VkQueueFamilyProperties2 const& queueFamilyProperties : physicalDeviceQueueFamilyProperties) {
            if (queueFamilyProperties.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilyIndexWithGraphicsCapabilities = queueFamilyIndex; 
            }

            // check surface support for a specific queue family
            VkBool32 supportedSurface = false;
            VkResult getPhysicalDeviceSurfaceSupportKHRResult = vkGetPhysicalDeviceSurfaceSupportKHR(pd, queueFamilyIndex, surface, &supportedSurface);
            if (getPhysicalDeviceSurfaceSupportKHRResult != VK_SUCCESS) {
                std::cerr << "Could not get surface support (status: " << getPhysicalDeviceSurfaceSupportKHRResult << ")" << std::endl;
                return CleanOnExit(EXIT_FAILURE);
            }

            if (supportedSurface) {
                queueFamilyIndexWithPresentCapabilities = queueFamilyIndex;
            }

            //DisplayPhysicalDeviceQueueFamilyProperties(queueFamilyProperties.queueFamilyProperties);
        }

        // enumerate device extensions
        uint32_t deviceExtensionPropertiesCount = 0;
        VkResult enumerateDeviceExtensionPropertiesResult = vkEnumerateDeviceExtensionProperties(pd, VK_NULL_HANDLE, &deviceExtensionPropertiesCount, VK_NULL_HANDLE);
        if (enumerateDeviceExtensionPropertiesResult != VK_SUCCESS) {
            std::cerr << "Could not get device extensions properties (1st call, status: " << enumerateDeviceExtensionPropertiesResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Device extensions properties enumerated successfully (1st call, count: " << deviceExtensionPropertiesCount << ")" << std::endl;

        std::vector<VkExtensionProperties> deviceExtensionsProperties(deviceExtensionPropertiesCount, VkExtensionProperties {});
        enumerateDeviceExtensionPropertiesResult = vkEnumerateDeviceExtensionProperties(pd, VK_NULL_HANDLE, &deviceExtensionPropertiesCount, deviceExtensionsProperties.data());
        if (enumerateDeviceExtensionPropertiesResult != VK_SUCCESS) {
            std::cerr << "Could not get device extensions properties (2nd call, status: " << enumerateDeviceExtensionPropertiesResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Device extensions properties enumerated successfully (2nd call, retrieved in array)" << std::endl;

        std::set<std::string> requiredDeviceExtensions(deviceExtensions.begin(), deviceExtensions.end());
        for (VkExtensionProperties const& deviceExtensionProperties : deviceExtensionsProperties) {
            requiredDeviceExtensions.erase(deviceExtensionProperties.extensionName);
        }

        bool deviceExtensionsSupported = requiredDeviceExtensions.empty();

        if (deviceExtensionsSupported) {
            std::clog << "All device extensions are supported" << std::endl;
        }

        else {
            std::clog << "Some or all device extensions aren't supported" << std::endl;
        }

        // get capabilities of the surface
        VkSurfaceCapabilities2KHR surfaceCapabilities {
            .sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR
        };
        VkPhysicalDeviceSurfaceInfo2KHR physicalDeviceSurfaceInfo {};
        physicalDeviceSurfaceInfo.pNext = nullptr;
        physicalDeviceSurfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
        physicalDeviceSurfaceInfo.surface = surface;
        
        VkResult getPhysicalDeviceSurfaceCapabilitiesResult = vkGetPhysicalDeviceSurfaceCapabilities2KHR(pd, &physicalDeviceSurfaceInfo, &surfaceCapabilities);
        if (getPhysicalDeviceSurfaceCapabilitiesResult != VK_SUCCESS) {
            std::cerr << "Could not get physical device surface capabilities (status: " << enumerateDeviceExtensionPropertiesResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Physical device surface capabilities retrieved successfully" << std::endl;

        // get formats of the surface
        uint32_t surfaceFormatsCount = 0;
        VkResult getPhysicalDeviceSurfaceFormats2KHRResult = vkGetPhysicalDeviceSurfaceFormats2KHR(pd, &physicalDeviceSurfaceInfo, &surfaceFormatsCount, VK_NULL_HANDLE);
        if (getPhysicalDeviceSurfaceFormats2KHRResult != VK_SUCCESS) {
            std::cerr << "Could not get physical device surface formats (1st call, status: " << getPhysicalDeviceSurfaceFormats2KHRResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Physical device surface formats retrieved successfully (1st call, count: " << surfaceFormatsCount << ")" << std::endl;

        if (surfaceFormatsCount == 0) {
            std::cerr << "No surface formats found" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }

        std::vector<VkSurfaceFormat2KHR> surfaceFormats(
            surfaceFormatsCount,
            VkSurfaceFormat2KHR {
                .sType = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR
            }
        );
        getPhysicalDeviceSurfaceFormats2KHRResult = vkGetPhysicalDeviceSurfaceFormats2KHR(pd, &physicalDeviceSurfaceInfo, &surfaceFormatsCount, surfaceFormats.data());
        if (getPhysicalDeviceSurfaceFormats2KHRResult != VK_SUCCESS) {
            std::cerr << "Could not get physical device surface formats (2nd call, status: " << getPhysicalDeviceSurfaceFormats2KHRResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Physical device surface formats retrieved successfully (2nd call, retrieved in array)" << std::endl;

        // get present modes of the surface
        uint32_t presentModesCount = 0;
        VkResult getPhysicalDeviceSurfacePresentModesKHR = vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &presentModesCount, VK_NULL_HANDLE);
        if (getPhysicalDeviceSurfacePresentModesKHR != VK_SUCCESS) {
            std::cerr << "Could not get physical device surface present modes (1st call, status: " << getPhysicalDeviceSurfaceFormats2KHRResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Physical device surface present modes retrieved successfully (1st call, count: " << presentModesCount << ")" << std::endl;

        if (presentModesCount == 0) {
            std::cerr << "No present modes found" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }

        std::vector<VkPresentModeKHR> presentModes(presentModesCount, VkPresentModeKHR {});
        getPhysicalDeviceSurfacePresentModesKHR = vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &presentModesCount, presentModes.data());
        if (getPhysicalDeviceSurfacePresentModesKHR != VK_SUCCESS) {
            std::cerr << "Could not get physical device surface present modes (2nd call, status: " << getPhysicalDeviceSurfaceFormats2KHRResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Physical device surface present modes retrieved successfully (2nd call, retrieved in array)" << std::endl;

        // check is swapchain has mandatory properties
        bool swapChainIsAdequate = false;
        if (deviceExtensionsSupported) {
            swapChainIsAdequate = 
                !surfaceFormats.empty() &&
                !presentModes.empty()
                ;
        }

        preferredFormat = surfaceFormats[0];
        for (VkSurfaceFormat2KHR const& surfaceFormat : surfaceFormats) {
            if (
                surfaceFormat.surfaceFormat.format     == VK_FORMAT_B8G8R8A8_SRGB           &&
                surfaceFormat.surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            ) {
                preferredFormat = surfaceFormat;
            }
        }

        preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (VkPresentModeKHR const& presentMode : presentModes) {
            if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                preferredPresentMode = presentMode;
            }
        }

        if (surfaceCapabilities.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            swapchainExtent = surfaceCapabilities.surfaceCapabilities.currentExtent;
        }

        else {
            int width = 0;
            int height = 0;
            if (!SDL_GetWindowSizeInPixels(window, &width, &height)) {
                std::cerr << "Couldn't get window size: " << SDL_GetError() << std::endl;
                swapchainExtent = surfaceCapabilities.surfaceCapabilities.currentExtent;
            }

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width  = std::clamp(actualExtent.width,
                                             surfaceCapabilities.surfaceCapabilities.minImageExtent.width,
                                             surfaceCapabilities.surfaceCapabilities.maxImageExtent.width);

            actualExtent.height = std::clamp(actualExtent.height,
                                             surfaceCapabilities.surfaceCapabilities.minImageExtent.height,
                                             surfaceCapabilities.surfaceCapabilities.maxImageExtent.height);
            
            swapchainExtent = actualExtent;
        }

        imageCount = surfaceCapabilities.surfaceCapabilities.minImageCount + 1;

        if (
            surfaceCapabilities.surfaceCapabilities.maxImageCount > 0          &&
            imageCount > surfaceCapabilities.surfaceCapabilities.maxImageCount
        ) {
            imageCount = surfaceCapabilities.surfaceCapabilities.maxImageCount;
        }

        currentTransform = surfaceCapabilities.surfaceCapabilities.currentTransform;
 
        //DisplayPhysicalDeviceProperties(pd, physicalDeviceProperties);
        //DisplayPhysicalDeviceFeatures(pd, physicalDeviceFeatures);
        //DisplayPhysicalDeviceSurfaceCapabilities(surfaceCapabilities.surfaceCapabilities);

        // check if device is suitable
        if (std::string deviceName = std::string(physicalDeviceProperties.properties.deviceName);
            physicalDeviceProperties.properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
         && deviceName.find("Microsoft Direct3D") == std::string::npos
         && physicalDeviceFeatures.features.geometryShader
         && queueFamilyIndexWithGraphicsCapabilities.has_value()
         && queueFamilyIndexWithPresentCapabilities.has_value()
         && deviceExtensionsSupported
         && swapChainIsAdequate
        ) {
            std::clog << "Current physical device selected" << std::endl;
            physicalDevice = pd;
            DisplayPhysicalDeviceProperties(physicalDeviceProperties, true, false);
            DisplayPhysicalDeviceFeatures(physicalDeviceFeatures);
            queueFamilyIndex = 0;
            std::clog << "\t - Queue families (count: " << physicalDeviceQueueFamilyPropertiesCount << ")" << std::endl;
            for (VkQueueFamilyProperties2 const& queueFamilyProperties : physicalDeviceQueueFamilyProperties) {
                std::clog << "\t\t - [" << queueFamilyIndex++ << "]: " << std::endl;
                DisplayPhysicalDeviceQueueFamilyProperties(queueFamilyProperties.queueFamilyProperties);            
            }

            DisplayPhysicalDeviceSurfaceCapabilities(surfaceCapabilities.surfaceCapabilities);

            std::clog << "\t - Surface formats:" << std::endl;
            for (VkSurfaceFormat2KHR const& surfaceFormat : surfaceFormats) {
                std::clog << "\t\t - Surface format found:" << std::endl;
                DisplaySurfaceFormat(surfaceFormat.surfaceFormat);
            }

            std::clog << "\t - Present modes:" << std::endl;
            for (VkPresentModeKHR const& presentMode : presentModes) {
                DisplayPresentMode(presentMode);
            }

            std::clog << "\t - Queue has graphics capabilities index:     " 
                      << queueFamilyIndexWithGraphicsCapabilities.value() << std::endl;
            std::clog << "\t - Queue has present capabilities index:      "
                      << queueFamilyIndexWithPresentCapabilities.value() << std::endl;
        }
    }
    std::clog.flags(formatFlags);

    if (physicalDevice == VK_NULL_HANDLE || !queueFamilyIndexWithGraphicsCapabilities.has_value() || !queueFamilyIndexWithPresentCapabilities.has_value()) {
        std::cerr << "Unable to get a suitable physical device" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }

    // specify queues create info
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {}; 
    std::set<uint32_t> uniqueQueueFamilies = {
        queueFamilyIndexWithGraphicsCapabilities.value(),
        queueFamilyIndexWithPresentCapabilities.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo deviceQueueCreateInfo {};
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.pNext = VK_NULL_HANDLE;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos.push_back(deviceQueueCreateInfo);
    }

    // specify device enabled features (nothing for now)
    VkPhysicalDeviceFeatures2 physicalDeviceFeatures {};

    // create device
    VkDeviceCreateInfo deviceCreateInfo {};
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.enabledLayerCount = (enableValidationLayers ? static_cast<uint32_t>(validationLayers.size()) : 0);
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures.features;
    deviceCreateInfo.pNext = VK_NULL_HANDLE;
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.ppEnabledLayerNames = (enableValidationLayers ? validationLayers.data() : VK_NULL_HANDLE);
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    VkResult deviceCreateResult = vkCreateDevice(physicalDevice, &deviceCreateInfo, VK_NULL_HANDLE, &device);
    if (deviceCreateResult != VK_SUCCESS) {
        std::cerr << "Unable to create a device (status: " << enumeratePhysicalDeviceResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Device created successully: <VkDevice " << device << ">" << std::endl;

    // get device graphics queue
    VkDeviceQueueInfo2 deviceGraphicsQueueInfo {};
    deviceGraphicsQueueInfo.flags = 0;
    deviceGraphicsQueueInfo.pNext = VK_NULL_HANDLE;
    deviceGraphicsQueueInfo.queueFamilyIndex = queueFamilyIndexWithGraphicsCapabilities.value();
    deviceGraphicsQueueInfo.queueIndex = 0;
    deviceGraphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    vkGetDeviceQueue2(device, &deviceGraphicsQueueInfo, &graphicsQueue);
    std::clog << "Graphics queue retrieved succeffully" << std::endl;

    // get device present queue
    VkDeviceQueueInfo2 devicePresentQueueInfo {};
    devicePresentQueueInfo.flags = 0;
    devicePresentQueueInfo.pNext = VK_NULL_HANDLE;
    devicePresentQueueInfo.queueFamilyIndex = queueFamilyIndexWithPresentCapabilities.value();
    devicePresentQueueInfo.queueIndex = 0;
    devicePresentQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    vkGetDeviceQueue2(device, &devicePresentQueueInfo, &presentQueue);
    std::clog << "Present queue retrieved succeffully" << std::endl;

    // create swapchain
    VkSwapchainCreateInfoKHR swapChainCreateInfo {};
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.flags = 0;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageColorSpace = preferredFormat.surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = swapchainExtent;
    swapChainCreateInfo.imageFormat = preferredFormat.surfaceFormat.format;
    
    std::vector<uint32_t> queueFamilyIndices = {
        queueFamilyIndexWithGraphicsCapabilities.value(),
        queueFamilyIndexWithPresentCapabilities.value()
    };

    if (queueFamilyIndexWithGraphicsCapabilities.value() != queueFamilyIndexWithPresentCapabilities.value()) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }

    else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = VK_NULL_HANDLE;
    }

    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    swapChainCreateInfo.pNext = VK_NULL_HANDLE;
    swapChainCreateInfo.presentMode = preferredPresentMode;
    swapChainCreateInfo.preTransform = currentTransform;
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;

    VkResult createSwapChainResult = vkCreateSwapchainKHR(device, &swapChainCreateInfo, VK_NULL_HANDLE, &swapChain);
    if (createSwapChainResult != VK_SUCCESS) {
        std::cerr << "Unable to create a swapChain (status: " << createSwapChainResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Swap chain created successully: <VkSwapchainKHR " << device << ">" << std::endl;

    // retrieve swap chain images
    uint32_t swapChainImageCount = 0;
    VkResult getSwapChainImagesResult = vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, VK_NULL_HANDLE);
    if (getSwapChainImagesResult != VK_SUCCESS) {
        std::cerr << "Unable to retrieve the swap chain images (1st call, status: " << getSwapChainImagesResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Swap chain images retrieved successully (1st call, count: " << swapChainImageCount << ")" << std::endl;

    std::vector<VkImage> swapChainImages(swapChainImageCount);
    getSwapChainImagesResult = vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
    if (getSwapChainImagesResult != VK_SUCCESS) {
        std::cerr << "Unable to retrieve the swap chain images (2nd call, status: " << getSwapChainImagesResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Swap chain images retrieved successully (2nd call, retrieved in array)" << std::endl;

    // create swap chain images views
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); ++i) {
        // create swap chain image view
        VkImageViewCreateInfo swapChainImageViewCreateInfo {};
        swapChainImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        swapChainImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        swapChainImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        swapChainImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        swapChainImageViewCreateInfo.flags = 0;
        swapChainImageViewCreateInfo.format = preferredFormat.surfaceFormat.format;
        swapChainImageViewCreateInfo.image = swapChainImages[i];
        swapChainImageViewCreateInfo.pNext = VK_NULL_HANDLE;
        swapChainImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        swapChainImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        swapChainImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        swapChainImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        swapChainImageViewCreateInfo.subresourceRange.layerCount = 1;
        swapChainImageViewCreateInfo.subresourceRange.levelCount = 1;
        swapChainImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

        VkResult createImageViewResult = vkCreateImageView(device, &swapChainImageViewCreateInfo, VK_NULL_HANDLE, &swapChainImageViews[i]);
        if (createImageViewResult != VK_SUCCESS) {
            std::cerr << "Unable to create a swap chain image view (status: " << createImageViewResult << ")" << std::endl;
            return CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Swap chain image view created successfully: <VkImageView " << swapChainImageViews[i] << ">" << std::endl;
    }

    // opening vertex shader file
    std::ifstream vertexShaderFile(
        "resources/shaders/triangle.vertex.spv",
        std::ios::ate | std::ios::binary
    );
    if (!vertexShaderFile.is_open()) {
        std::cerr << "Unable to open the vertex shader file" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Vertex shader file opened successfully" << std::endl;

    size_t vertexShaderFileSize = static_cast<size_t>(vertexShaderFile.tellg());
    std::vector<char> vertexShaderFileBuffer(vertexShaderFileSize);

    vertexShaderFile.seekg(0);
    vertexShaderFile.read(vertexShaderFileBuffer.data(), vertexShaderFileSize);

    vertexShaderFile.close();

    // opening fragment shader file
    std::ifstream fragmentShaderFile(
        "resources/shaders/triangle.fragment.spv",
        std::ios::ate | std::ios::binary
    );
    if (!fragmentShaderFile.is_open()) {
        std::cerr << "Unable to open the fragment shader file" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Fragment shader file opened successfully" << std::endl;

    size_t fragmentShaderFileSize = static_cast<size_t>(fragmentShaderFile.tellg());
    std::vector<char> fragmentShaderFileBuffer(fragmentShaderFileSize);

    fragmentShaderFile.seekg(0);
    fragmentShaderFile.read(fragmentShaderFileBuffer.data(), fragmentShaderFileSize);

    fragmentShaderFile.close();

    // create vertex shader module
    VkShaderModuleCreateInfo vertexShaderModuleCreateInfo {};
    vertexShaderModuleCreateInfo.codeSize = vertexShaderFileBuffer.size();
    vertexShaderModuleCreateInfo.flags = 0;
    vertexShaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t const*>(vertexShaderFileBuffer.data());
    vertexShaderModuleCreateInfo.pNext = VK_NULL_HANDLE;
    vertexShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    VkResult createVertexShaderModuleResult = vkCreateShaderModule(device, &vertexShaderModuleCreateInfo, VK_NULL_HANDLE, &vertexShaderModule);
    if (createVertexShaderModuleResult != VK_SUCCESS) {
        std::cerr << "Unable to create the vertex shader module: (status: " << createVertexShaderModuleResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::cerr << "Vertex shader module created successfully: <VkShaderModule " << vertexShaderModule << ">" << std::endl;

    // create fragment shader module
    VkShaderModuleCreateInfo fragmentShaderModuleCreateInfo {};
    fragmentShaderModuleCreateInfo.codeSize = fragmentShaderFileBuffer.size();
    fragmentShaderModuleCreateInfo.flags = 0;
    fragmentShaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t const*>(fragmentShaderFileBuffer.data());
    fragmentShaderModuleCreateInfo.pNext = VK_NULL_HANDLE;
    fragmentShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    VkResult createFragmentShaderModuleResult = vkCreateShaderModule(device, &fragmentShaderModuleCreateInfo, VK_NULL_HANDLE, &fragmentShaderModule);
    if (createFragmentShaderModuleResult != VK_SUCCESS) {
        std::cerr << "Unable to create the fragment shader module: (status: " << createFragmentShaderModuleResult << ")" << std::endl;
        return CleanOnExit(EXIT_FAILURE);
    }
    std::cerr << "Fragment shader module created successfully: <VkShaderModule " << fragmentShaderModule << ">" << std::endl;

    // create vertex shader pipeline stage
    VkPipelineShaderStageCreateInfo vertexShaderPipelineStageCreateInfo {};
    vertexShaderPipelineStageCreateInfo.flags = 0;
    vertexShaderPipelineStageCreateInfo.module = vertexShaderModule;
    vertexShaderPipelineStageCreateInfo.pName = "main";
    vertexShaderPipelineStageCreateInfo.pNext = VK_NULL_HANDLE;
    vertexShaderPipelineStageCreateInfo.pSpecializationInfo = VK_NULL_HANDLE;
    vertexShaderPipelineStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderPipelineStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    // create fragment shader pipeline stage
    VkPipelineShaderStageCreateInfo fragmentShaderPipelineStageCreateInfo {};
    fragmentShaderPipelineStageCreateInfo.flags = 0;
    fragmentShaderPipelineStageCreateInfo.module = fragmentShaderModule;
    fragmentShaderPipelineStageCreateInfo.pName = "main";
    fragmentShaderPipelineStageCreateInfo.pNext = VK_NULL_HANDLE;
    fragmentShaderPipelineStageCreateInfo.pSpecializationInfo = VK_NULL_HANDLE;
    fragmentShaderPipelineStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderPipelineStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos = {
        vertexShaderPipelineStageCreateInfo,
        fragmentShaderPipelineStageCreateInfo
    };

    // specify pipeline dynamic states
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo {};
    pipelineDynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    pipelineDynamicStateCreateInfo.flags = 0;
    pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates.data();
    pipelineDynamicStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

    // specify pipeline vertex inputs
    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo {};
    pipelineVertexInputStateCreateInfo.flags = 0;
    pipelineVertexInputStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = VK_NULL_HANDLE;
    pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = VK_NULL_HANDLE;
    pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
    pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;

    // specify pipeline input assemblies
    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo {};
    pipelineInputAssemblyStateCreateInfo.flags = 0;
    pipelineInputAssemblyStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    // specify viewport and scissor
    VkViewport viewport {};
    viewport.height = static_cast<float>(swapchainExtent.height);
    viewport.maxDepth = 1.0f;
    viewport.minDepth = 0.0f;
    viewport.width = static_cast<float>(swapchainExtent.width);
    viewport.x = 0.0f;
    viewport.y = 0.0f;

    VkRect2D scissor {};
    scissor.extent = swapchainExtent;
    scissor.offset = VkOffset2D { .x = 0, .y = 0 };

    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo {};
    pipelineViewportStateCreateInfo.flags = 0;
    pipelineViewportStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineViewportStateCreateInfo.pScissors = &scissor;
    pipelineViewportStateCreateInfo.pViewports = &viewport;
    pipelineViewportStateCreateInfo.scissorCount = 1;
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.viewportCount = 1;

    // specify pipeline rasterization state
    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo {};
    pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
    pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.flags = 0;
    pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
    pipelineRasterizationStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

    // specify pipeline multisampling state
    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo {};
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.flags = 0;
    pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
    pipelineMultisampleStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineMultisampleStateCreateInfo.pSampleMask = VK_NULL_HANDLE;
    pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

    // specify pipeline depth stencil state
    /*
    VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo {};
    pipelineDepthStencilStateCreateInfo.back = VkStencilOpState {};
    pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS;
    pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_FALSE;
    pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_FALSE;
    pipelineDepthStencilStateCreateInfo.flags = 0;
    pipelineDepthStencilStateCreateInfo.front = VkStencilOpState {};
    pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0f;
    pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
    pipelineDepthStencilStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    */

    // specify pipeline color blend attachment state
    VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState {};
    pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
    pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;

    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo {};
    pipelineColorBlendStateCreateInfo.attachmentCount = 1;
    pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;
    pipelineColorBlendStateCreateInfo.flags = 0;
    pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
    pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
    pipelineColorBlendStateCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

    // create pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo {};
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.pNext = VK_NULL_HANDLE;
    pipelineLayoutCreateInfo.pPushConstantRanges = VK_NULL_HANDLE;
    pipelineLayoutCreateInfo.pSetLayouts = VK_NULL_HANDLE;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    VkResult createPipelineLayoutResult = vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, VK_NULL_HANDLE, &pipelineLayout);
    if (createPipelineLayoutResult != VK_SUCCESS) {
        std::cerr << "Unable to create a pipeline layout (status: " << createPipelineLayoutResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Pipeline layout created successfully: <VkPipelineLayout " << pipelineLayout << ">" << std::endl; 

    // create render pass
    VkAttachmentDescription2 attachmentDescription {};
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachmentDescription.flags = 0;
    attachmentDescription.format = preferredFormat.surfaceFormat.format;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.pNext = VK_NULL_HANDLE;
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2;

    VkAttachmentReference2 attachmentReference {};
    attachmentReference.aspectMask = VK_IMAGE_ASPECT_NONE;
    attachmentReference.attachment = 0;
    attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentReference.pNext = VK_NULL_HANDLE;
    attachmentReference.sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;

    VkSubpassDescription2 subpassDescription {};
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.flags = 0;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pColorAttachments = &attachmentReference;
    subpassDescription.pDepthStencilAttachment = VK_NULL_HANDLE;
    subpassDescription.pInputAttachments = VK_NULL_HANDLE;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.pNext = VK_NULL_HANDLE;
    subpassDescription.pPreserveAttachments = VK_NULL_HANDLE;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pResolveAttachments = VK_NULL_HANDLE;
    subpassDescription.sType = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2;
    subpassDescription.viewMask = 0;

    // specify subpass dependencies
    VkSubpassDependency2 subpassDependency {};
    subpassDependency.dependencyFlags = 0;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstSubpass = 0;
    subpassDependency.pNext = VK_NULL_HANDLE;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.sType = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2;
    subpassDependency.viewOffset = 0;

    VkRenderPassCreateInfo2 renderPassCreateInfo {};
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.correlatedViewMaskCount = 0;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.pCorrelatedViewMasks = VK_NULL_HANDLE;
    renderPassCreateInfo.pDependencies = &subpassDependency;
    renderPassCreateInfo.pNext = VK_NULL_HANDLE;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2;
    renderPassCreateInfo.subpassCount = 1;

    VkResult createRenderPassResult = vkCreateRenderPass2(device, &renderPassCreateInfo, VK_NULL_HANDLE, &renderPass);
    if (createRenderPassResult != VK_SUCCESS) {
        std::cerr << "Could not create a render pass (status: " << createRenderPassResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Render pass created successfully: <VkRenderPass " << renderPass << ">" << std::endl;

    // create graphics pipeline
    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo {};
    graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.basePipelineIndex = -1;
    graphicsPipelineCreateInfo.flags = 0;
    graphicsPipelineCreateInfo.layout = pipelineLayout;
    graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
    graphicsPipelineCreateInfo.pNext = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
    graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos.data();
    graphicsPipelineCreateInfo.pTessellationState = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
    graphicsPipelineCreateInfo.renderPass = renderPass;
    graphicsPipelineCreateInfo.stageCount = 2;
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.subpass = 0;

    VkResult createPipelineResult = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, VK_NULL_HANDLE, &graphicsPipeline);
    if (createPipelineResult != VK_SUCCESS) {
        std::cerr << "Could not create a graphics pipeline (status: " << createPipelineResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Graphics pipeline created successfully: <VkPipeline " << graphicsPipeline << ">" << std::endl;

    frameBuffers.resize(swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
        // create frame buffer
        std::vector<VkImageView> attachments = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo frameBufferCreateInfo {};
        frameBufferCreateInfo.attachmentCount = 1;
        frameBufferCreateInfo.flags = 0;
        frameBufferCreateInfo.height = swapchainExtent.height;
        frameBufferCreateInfo.layers = 1;
        frameBufferCreateInfo.pAttachments = attachments.data();
        frameBufferCreateInfo.pNext = VK_NULL_HANDLE;
        frameBufferCreateInfo.renderPass = renderPass;
        frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferCreateInfo.width = swapchainExtent.width;

        VkResult createFramebufferResult = vkCreateFramebuffer(device, &frameBufferCreateInfo, VK_NULL_HANDLE, &frameBuffers[i]);
        if (createFramebufferResult != VK_SUCCESS) {
            std::cerr << "Could not create frame buffer (status: " << createFramebufferResult << ")" << std::endl;
            CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Frame buffer created successfully: <VkFramebuffer " << frameBuffers[i] << ">" << std::endl;
    }

    // create command pool
    VkCommandPoolCreateInfo commandPoolCreateInfo {};
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.pNext = VK_NULL_HANDLE;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndexWithGraphicsCapabilities.value();
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    VkResult createCommandPoolResult = vkCreateCommandPool(device, &commandPoolCreateInfo, VK_NULL_HANDLE, &commandPool);
    if (createCommandPoolResult != VK_SUCCESS) {
        std::cerr << "Could not create command pool (status: " << createCommandPoolResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Command pool created successfully: <VkCommandPool " << commandPool << ">" << std::endl;

    // create command buffer
    VkCommandBufferAllocateInfo commandBufferAllocateInfo {};
    commandBufferAllocateInfo.commandBufferCount = 1;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.pNext = VK_NULL_HANDLE;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    VkResult allocateCommandBufferResult = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);
    if (allocateCommandBufferResult != VK_SUCCESS) {
        std::cerr << "Unable to allocate a command buffer (status: " << allocateCommandBufferResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "Command buffer allocated successfully: <VkCommandBuffer " << commandBuffer << ">" << std::endl;

    // create synchronization primitive objects
    VkSemaphoreCreateInfo semaphoreCreateInfo {};
    semaphoreCreateInfo.flags = 0;
    semaphoreCreateInfo.pNext = VK_NULL_HANDLE;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkResult createImageAvailableSemaphoreResult = vkCreateSemaphore(device, &semaphoreCreateInfo, VK_NULL_HANDLE, &imageAvailableSemaphore);
    if (createImageAvailableSemaphoreResult != VK_SUCCESS) {
        std::cerr << "Unable to create 'Image available' semaphore (status: " << createImageAvailableSemaphoreResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "'Image available' semaphore created successfully: <VkSemaphore " << imageAvailableSemaphore << ">" << std::endl;

    VkResult createRenderFinishedSemaphoreResult = vkCreateSemaphore(device, &semaphoreCreateInfo, VK_NULL_HANDLE, &renderFinishedSemaphore);
    if (createRenderFinishedSemaphoreResult != VK_SUCCESS) {
        std::cerr << "Unable to create 'Render finished' semaphore (status: " << createRenderFinishedSemaphoreResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "'Render finished' semaphore created successfully: <VkSemaphore " << renderFinishedSemaphore << ">" << std::endl;

    VkFenceCreateInfo fenceCreateInfo {};
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // to avoid deadlock on the first frame
    fenceCreateInfo.pNext = VK_NULL_HANDLE;
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    VkResult createInFlightFenceResult = vkCreateFence(device, &fenceCreateInfo, VK_NULL_HANDLE, &inFlightFence);
    if (createInFlightFenceResult != VK_SUCCESS) {
        std::cerr << "Unable to create 'In flight' fence (status: " << createInFlightFenceResult << ")" << std::endl;
        CleanOnExit(EXIT_FAILURE);
    }
    std::clog << "'Render finished' fence created successfully: <VkFence " << inFlightFence << ">" << std::endl;
    
    running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &inFlightFence);

        uint32_t imageIndex = 0;

        VkAcquireNextImageInfoKHR acquireNextImageInfo {};
        acquireNextImageInfo.deviceMask = 0;
        acquireNextImageInfo.fence = VK_NULL_HANDLE;
        acquireNextImageInfo.pNext = VK_NULL_HANDLE;
        acquireNextImageInfo.semaphore = imageAvailableSemaphore;
        acquireNextImageInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;
        acquireNextImageInfo.swapchain = swapChain;
        acquireNextImageInfo.timeout = UINT64_MAX;

        vkAcquireNextImage2KHR(device, &acquireNextImageInfo, &imageIndex);

        vkResetCommandBuffer(commandBuffer, 0);

        // begin command buffer recording
        VkCommandBufferBeginInfo commandBufferBeginInfo {};
        commandBufferBeginInfo.flags = 0;
        commandBufferBeginInfo.pInheritanceInfo = VK_NULL_HANDLE;
        commandBufferBeginInfo.pNext = VK_NULL_HANDLE;
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        VkResult beginCommandBufferResult = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
        if (beginCommandBufferResult != VK_SUCCESS) {
            std::cerr << "Unable to begin command buffer recording (status: " << beginCommandBufferResult << ")" << std::endl;
            CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Command buffer recording began successfully: <VkCommandBuffer " << commandBuffer << ">" << std::endl;

        VkClearValue clearValue {};
        clearValue.color.float32[0] = 0.0f;
        clearValue.color.float32[1] = 0.0f;
        clearValue.color.float32[2] = 0.0f;
        clearValue.color.float32[3] = 1.0f;
        clearValue.depthStencil.depth = 0.0f;
        clearValue.depthStencil.stencil = 0;

        VkRenderPassBeginInfo renderPassBeginInfo {};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.framebuffer = frameBuffers[imageIndex];
        renderPassBeginInfo.pClearValues = &clearValue;
        renderPassBeginInfo.pNext = VK_NULL_HANDLE;
        renderPassBeginInfo.renderArea.extent = swapchainExtent;
        renderPassBeginInfo.renderArea.offset.x = 0;
        renderPassBeginInfo.renderArea.offset.y = 0;
        renderPassBeginInfo.renderPass = renderPass;
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

        VkSubpassBeginInfo subpassBeginInfo {};
        subpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;
        subpassBeginInfo.pNext = VK_NULL_HANDLE;
        subpassBeginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;

        vkCmdBeginRenderPass2(commandBuffer, &renderPassBeginInfo, &subpassBeginInfo);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        VkSubpassEndInfo subpassEndInfo {};
        subpassEndInfo.pNext = VK_NULL_HANDLE;
        subpassEndInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO;

        vkCmdEndRenderPass2(commandBuffer, &subpassEndInfo);

        VkResult endCommandBufferResult = vkEndCommandBuffer(commandBuffer);
        if (endCommandBufferResult != VK_SUCCESS) {
            std::cerr << "Unable to end the command buffer (status: " << endCommandBufferResult << ")" << std::endl;
            CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Command buffer ended successfully" << std::endl;
        
        VkCommandBufferSubmitInfo commandBufferSubmitInfo {};
        commandBufferSubmitInfo.commandBuffer = commandBuffer;
        commandBufferSubmitInfo.deviceMask = 0;
        commandBufferSubmitInfo.pNext = VK_NULL_HANDLE;
        commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;

        VkSemaphoreSubmitInfo waitSemaphoreSubmitInfo {};
        waitSemaphoreSubmitInfo.deviceIndex = 0;
        waitSemaphoreSubmitInfo.pNext = VK_NULL_HANDLE;
        waitSemaphoreSubmitInfo.semaphore = imageAvailableSemaphore;
        waitSemaphoreSubmitInfo.stageMask = 0;
        waitSemaphoreSubmitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        waitSemaphoreSubmitInfo.value = 1;

        std::vector<VkSemaphore> waitSemaphores = { imageAvailableSemaphore };

        VkSemaphoreSubmitInfo signalSemaphoreSubmitInfo {};
        signalSemaphoreSubmitInfo.deviceIndex = 0;
        signalSemaphoreSubmitInfo.pNext = VK_NULL_HANDLE;
        signalSemaphoreSubmitInfo.semaphore = renderFinishedSemaphore;
        signalSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        signalSemaphoreSubmitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        signalSemaphoreSubmitInfo.value = 1;

        std::vector<VkSemaphore> signalSemaphores = { renderFinishedSemaphore };

        VkSubmitInfo2 submitInfo {};
        submitInfo.commandBufferInfoCount = 1;
        submitInfo.flags = 0;
        submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
        submitInfo.pNext = VK_NULL_HANDLE;
        submitInfo.pSignalSemaphoreInfos = &signalSemaphoreSubmitInfo;
        submitInfo.pWaitSemaphoreInfos = &waitSemaphoreSubmitInfo;
        submitInfo.signalSemaphoreInfoCount = 1;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
        submitInfo.waitSemaphoreInfoCount = 1;

        VkResult queueSubmitResult = vkQueueSubmit2(graphicsQueue, 1, &submitInfo, inFlightFence);
        if (queueSubmitResult != VK_SUCCESS) {
            std::cerr << "Unable to submit to queue (status " << queueSubmitResult << ")" << std::endl;
            CleanOnExit(EXIT_FAILURE);
        }
        std::clog << "Submitted to queue successfully" << std::endl;

        std::vector<VkSwapchainKHR> swapChains = { swapChain };

        VkPresentInfoKHR presentInfo {};
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pNext = VK_NULL_HANDLE;
        presentInfo.pResults = VK_NULL_HANDLE;
        presentInfo.pSwapchains = swapChains.data();
        presentInfo.pWaitSemaphores = signalSemaphores.data();
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.swapchainCount = static_cast<uint32_t>(swapChains.size());
        presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());

        vkQueuePresentKHR(graphicsQueue, &presentInfo);
    }

    //vkDeviceWaitIdle(device);

    return CleanOnExit(EXIT_SUCCESS);
}
