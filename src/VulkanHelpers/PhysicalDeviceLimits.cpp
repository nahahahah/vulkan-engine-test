#include "VulkanHelpers/PhysicalDeviceLimits.hpp"

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceLimits const& limits) {
    out << std::boolalpha;
    out << "\t\tBuffer image granularity                                 "
        << limits.bufferImageGranularity << std::endl;
    out << "\t\tDiscrete queue priorities                                "
        << limits.discreteQueuePriorities << std::endl;
    out << "\t\tFramebuffer color sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferColorSampleCounts);
    out << "\t\tFramebuffer depth sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferDepthSampleCounts);
    out << "\t\tFramebuffer no attachments sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferNoAttachmentsSampleCounts);
    out << "\t\tFramebuffer stencil sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferStencilSampleCounts);
    out << "\t\tLine width granularity                                   "
        << limits.lineWidthGranularity << std::endl;
    out << "\t\tLine width min range                                     "
        << limits.lineWidthRange[0] << std::endl;
    out << "\t\tLine width max range                                     "
        << limits.lineWidthRange[1] << std::endl;
    out << "\t\tMax bound descriptor sets                                "
        << limits.maxBoundDescriptorSets << std::endl;
    out << "\t\tMax clip distances                                       "
        << limits.maxClipDistances << std::endl;
    out << "\t\tMax color attachments                                    "
        << limits.maxColorAttachments << std::endl;
    out << "\t\tMax combined clip and cull distances                     "
        << limits.maxCombinedClipAndCullDistances << std::endl;
    out << "\t\tMax compute shared memory size                           "
        << limits.maxComputeSharedMemorySize << std::endl;
    out << "\t\tMax compute work group count X                           "
        << limits.maxComputeWorkGroupCount[0] << std::endl;
    out << "\t\tMax compute work group count Y                           "
        << limits.maxComputeWorkGroupCount[1] << std::endl;
    out << "\t\tMax compute work group count Z                           "
        << limits.maxComputeWorkGroupCount[2] << std::endl;
    out << "\t\tMax compute work group invocations                       "
        << limits.maxComputeWorkGroupInvocations << std::endl;
    out << "\t\tMax compute work group size X                            "
        << limits.maxComputeWorkGroupSize[0] << std::endl;
    out << "\t\tMax compute work group size Y                            "
        << limits.maxComputeWorkGroupSize[1] << std::endl;
    out << "\t\tMax compute work group size Z                            "
        << limits.maxComputeWorkGroupSize[2] << std::endl;
    out << "\t\tMax cull distances                                       "
        << limits.maxCullDistances << std::endl;
    out << "\t\tMax descriptor set input attachments                     "
        << limits.maxDescriptorSetInputAttachments << std::endl;
    out << "\t\tMax descriptor set sampled images                        "
        << limits.maxDescriptorSetSampledImages << std::endl;
    out << "\t\tMax descriptor set samplers                              "
        << limits.maxDescriptorSetSamplers << std::endl;
    out << "\t\tMax descriptor set storage buffers                       "
        << limits.maxDescriptorSetStorageBuffers << std::endl;
    out << "\t\tMax descriptor set storage buffers dynamic               "
        << limits.maxDescriptorSetStorageBuffersDynamic << std::endl;
    out << "\t\tMax descriptor set storage images                        "
        << limits.maxDescriptorSetStorageImages << std::endl;
    out << "\t\tMax descriptor set uniform buffers                       "
        << limits.maxDescriptorSetUniformBuffers << std::endl;
    out << "\t\tMax descriptor set uniform buffers dynamic               "
        << limits.maxDescriptorSetUniformBuffersDynamic << std::endl;
    out << "\t\tMax draw indexed index value                             "
        << limits.maxDrawIndexedIndexValue << std::endl;
    out << "\t\tMax draw indirect count                                  "
        << limits.maxDrawIndirectCount << std::endl;
    out << "\t\tMax fragment combined output resources                   "
        << limits.maxFragmentCombinedOutputResources << std::endl;
    out << "\t\tMax fragment dual source attachments                     "
        << limits.maxFragmentDualSrcAttachments << std::endl;
    out << "\t\tMax fragment input components                            "
        << limits.maxFragmentInputComponents << std::endl;
    out << "\t\tMax fragment output components                           "
        << limits.maxFragmentOutputAttachments << std::endl;
    out << "\t\tMax framebuffer height                                   "
        << limits.maxFramebufferHeight << std::endl;
    out << "\t\tMax framebuffer layers                                   "
        << limits.maxFramebufferLayers << std::endl;
    out << "\t\tMax framebuffer width                                    "
        << limits.maxFramebufferWidth << std::endl;
    out << "\t\tMax geometry input components                            "
        << limits.maxGeometryInputComponents << std::endl;
    out << "\t\tMax geometry output components                           "
        << limits.maxGeometryOutputComponents << std::endl;
    out << "\t\tMax geometry output vertices                             "
        << limits.maxGeometryOutputVertices << std::endl;
    out << "\t\tMax geometry shader invocations                          "
        << limits.maxGeometryShaderInvocations << std::endl;
    out << "\t\tMax geometry total output components                     "
        << limits.maxGeometryTotalOutputComponents << std::endl;
    out << "\t\tMax image array layers                                   "
        << limits.maxImageArrayLayers << std::endl;
    out << "\t\tMax image dimension 1D                                   "
        << limits.maxImageDimension1D << std::endl;
    out << "\t\tMax image dimension 2D                                   "
        << limits.maxImageDimension2D << std::endl;
    out << "\t\tMax image dimension 3D                                   "
        << limits.maxImageDimension3D << std::endl;
    out << "\t\tMax image dimension cube                                 "
        << limits.maxImageDimensionCube << std::endl;
    out << "\t\tMax interpolation offset                                 "
        << limits.maxInterpolationOffset << std::endl;
    out << "\t\tMax memory allocation count                              "
        << limits.maxMemoryAllocationCount << std::endl;
    out << "\t\tMax per stage descriptor input attachments               "
        << limits.maxPerStageDescriptorInputAttachments << std::endl;
    out << "\t\tMax per stage descriptor sampled images                  "
        << limits.maxPerStageDescriptorSampledImages << std::endl;
    out << "\t\tMax per stage descriptor samplers                        "
        << limits.maxPerStageDescriptorSamplers << std::endl;
    out << "\t\tMax per stage descriptor storage buffers                 "
        << limits.maxPerStageDescriptorStorageBuffers << std::endl;
    out << "\t\tMax per stage descriptor storage images                  "
        << limits.maxPerStageDescriptorStorageImages << std::endl;
    out << "\t\tMax per stage descriptor uniform buffers                 "
        << limits.maxPerStageDescriptorUniformBuffers << std::endl;
    out << "\t\tMax per stage resources                                  "
        << limits.maxPerStageResources << std::endl;
    out << "\t\tMax push constants size                                  "
        << limits.maxPushConstantsSize << std::endl;
    out << "\t\tMax sample mask words                                    "
        << limits.maxSampleMaskWords << std::endl;
    out << "\t\tMax sampler allocation count                             "
        << limits.maxSamplerAllocationCount << std::endl;
    out << "\t\tMax sampler anisotropy                                   "
        << limits.maxSamplerAnisotropy << std::endl;
    out << "\t\tMax sampler LOD bias                                     "
        << limits.maxSamplerLodBias << std::endl;
    out << "\t\tMax storage buffer range                                 "
        << limits.maxStorageBufferRange << std::endl;
    out << "\t\tMax tesselation control per patch output components      "
        << limits.maxTessellationControlPerPatchOutputComponents << std::endl;
    out << "\t\tMax tesselation control per vertex input components      "
        << limits.maxTessellationControlPerVertexInputComponents << std::endl;
    out << "\t\tMax tesselation control per vertex output components     "
        << limits.maxTessellationControlPerVertexOutputComponents << std::endl;
    out << "\t\tMax tesselation control per total output components      "
        << limits.maxTessellationControlTotalOutputComponents << std::endl;
    out << "\t\tMax tesselation evaluation input components              "
        << limits.maxTessellationEvaluationInputComponents << std::endl;
    out << "\t\tMax tesselation evaluation output components             "
        << limits.maxTessellationEvaluationOutputComponents << std::endl;
    out << "\t\tMax tesselation generation level                         "
        << limits.maxTessellationGenerationLevel << std::endl;
    out << "\t\tMax tesselation generation patch size                    "
        << limits.maxTessellationPatchSize << std::endl;
    out << "\t\tMax texel buffer elements                                "
        << limits.maxTexelBufferElements << std::endl;
    out << "\t\tMax texel gather offset                                  "
        << limits.maxTexelGatherOffset << std::endl;
    out << "\t\tMax texel offset                                         "
        << limits.maxTexelOffset << std::endl;
    out << "\t\tMax uniform buffer range                                 "
        << limits.maxUniformBufferRange << std::endl;
    out << "\t\tMax vertex input attribute offset                        "
        << limits.maxVertexInputAttributeOffset << std::endl;
    out << "\t\tMax vertex input attributes                              "
        << limits.maxVertexInputAttributes << std::endl;
    out << "\t\tMax vertex input bindings                                "
        << limits.maxVertexInputBindings << std::endl;
    out << "\t\tMax vertex input binding stride                          "
        << limits.maxVertexInputBindingStride << std::endl;
    out << "\t\tMax vertex output components                             "
        << limits.maxVertexOutputComponents << std::endl;
    out << "\t\tMax viewport dimension X                                 "
        << limits.maxViewportDimensions[0] << std::endl;
    out << "\t\tMax viewport dimension Y                                 "
        << limits.maxViewportDimensions[1] << std::endl;
    out << "\t\tMax viewports                                            "
        << limits.maxViewports << std::endl;
    out << "\t\tMin interpolation offset                                 "
        << limits.minInterpolationOffset << std::endl;
    out << "\t\tMin memory map alignment                                 "
        << limits.minMemoryMapAlignment << std::endl;
    out << "\t\tMin storage buffer offset alignment                      "
        << limits.minStorageBufferOffsetAlignment << std::endl;
    out << "\t\tMin texel buffer offset alignment                        "
        << limits.minTexelBufferOffsetAlignment << std::endl;
    out << "\t\tMin texel gather offset                                  "
        << limits.minTexelGatherOffset << std::endl;
    out << "\t\tMin texel offset                                         "
        << limits.minTexelOffset << std::endl;
    out << "\t\tMin uniform buffer offset alignment                      "
        << limits.minUniformBufferOffsetAlignment << std::endl;
    out << "\t\tMipmap precision bits                                    "
        << limits.mipmapPrecisionBits << std::endl;
    out << "\t\tNon coherent atom size                                   "
        << limits.nonCoherentAtomSize << std::endl;
    out << "\t\tOptimal buffer copy offset alignment                     "
        << limits.optimalBufferCopyOffsetAlignment << std::endl;
    out << "\t\tOptimal buffer copy row pitch alignment                  "
        << limits.optimalBufferCopyRowPitchAlignment << std::endl;
    out << "\t\tPoint size granularity                                   "
        << limits.pointSizeGranularity << std::endl;
    out << "\t\tPoint size min range                                     "
        << limits.pointSizeRange[0] << std::endl;
    out << "\t\tPoint size max range                                     "
        << limits.pointSizeRange[1] << std::endl;
    out << "\t\tSampled image color sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageColorSampleCounts);
    out << "\t\tSampled image depth sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageDepthSampleCounts);
    out << "\t\tSampled image integer sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageIntegerSampleCounts);
    out << "\t\tSampled image stencil sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageStencilSampleCounts);
    out << "\t\tSparse address space size                                "
        << limits.sparseAddressSpaceSize << std::endl;
    out << "\t\tStandard sample locations                                "
        << static_cast<bool>(limits.standardSampleLocations) << std::endl;
    out << "\t\tStandard image sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.storageImageSampleCounts);
    out << "\t\tStrict lines                                             "
        << static_cast<bool>(limits.strictLines) << std::endl;
    out << "\t\tSub pixel interpolation offset bits                      "
        << limits.subPixelInterpolationOffsetBits << std::endl;
    out << "\t\tSub pixel precision bits                                 "
        << limits.subPixelPrecisionBits << std::endl;
    out << "\t\tSub texel precision bits                                 "
        << limits.subTexelPrecisionBits << std::endl;
    out << "\t\tTimestamp compute and graphics                           "
        << static_cast<bool>(limits.timestampComputeAndGraphics) << std::endl;
    out << "\t\tTimestamp period                                         "
        << limits.timestampPeriod << std::endl;
    out << "\t\tViewport bounds min range                                "
        << limits.viewportBoundsRange[0] << std::endl;
    out << "\t\tViewport bounds max range                                "
        << limits.viewportBoundsRange[1] << std::endl;
    out << "\t\tViewport sub pixel bits                                  "
        << limits.viewportSubPixelBits << std::endl;
    out << std::noboolalpha;

    return out;
}