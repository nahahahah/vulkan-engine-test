#include "VulkanHelpers/ParameterStructs/PhysicalDeviceLimits.hpp"

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceLimits const& limits) {
    out << std::boolalpha;
    out << "Buffer image granularity                                 "
        << limits.bufferImageGranularity << std::endl;
    out << "Discrete queue priorities                                "
        << limits.discreteQueuePriorities << std::endl;
    out << "Framebuffer color sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferColorSampleCounts);
    out << "Framebuffer depth sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferDepthSampleCounts);
    out << "Framebuffer no attachments sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferNoAttachmentsSampleCounts);
    out << "Framebuffer stencil sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.framebufferStencilSampleCounts);
    out << "Line width granularity                                   "
        << limits.lineWidthGranularity << std::endl;
    out << "Line width min range                                     "
        << limits.lineWidthRange[0] << std::endl;
    out << "Line width max range                                     "
        << limits.lineWidthRange[1] << std::endl;
    out << "Max bound descriptor sets                                "
        << limits.maxBoundDescriptorSets << std::endl;
    out << "Max clip distances                                       "
        << limits.maxClipDistances << std::endl;
    out << "Max color attachments                                    "
        << limits.maxColorAttachments << std::endl;
    out << "Max combined clip and cull distances                     "
        << limits.maxCombinedClipAndCullDistances << std::endl;
    out << "Max compute shared memory size                           "
        << limits.maxComputeSharedMemorySize << std::endl;
    out << "Max compute work group count X                           "
        << limits.maxComputeWorkGroupCount[0] << std::endl;
    out << "Max compute work group count Y                           "
        << limits.maxComputeWorkGroupCount[1] << std::endl;
    out << "Max compute work group count Z                           "
        << limits.maxComputeWorkGroupCount[2] << std::endl;
    out << "Max compute work group invocations                       "
        << limits.maxComputeWorkGroupInvocations << std::endl;
    out << "Max compute work group size X                            "
        << limits.maxComputeWorkGroupSize[0] << std::endl;
    out << "Max compute work group size Y                            "
        << limits.maxComputeWorkGroupSize[1] << std::endl;
    out << "Max compute work group size Z                            "
        << limits.maxComputeWorkGroupSize[2] << std::endl;
    out << "Max cull distances                                       "
        << limits.maxCullDistances << std::endl;
    out << "Max descriptor set input attachments                     "
        << limits.maxDescriptorSetInputAttachments << std::endl;
    out << "Max descriptor set sampled images                        "
        << limits.maxDescriptorSetSampledImages << std::endl;
    out << "Max descriptor set samplers                              "
        << limits.maxDescriptorSetSamplers << std::endl;
    out << "Max descriptor set storage buffers                       "
        << limits.maxDescriptorSetStorageBuffers << std::endl;
    out << "Max descriptor set storage buffers dynamic               "
        << limits.maxDescriptorSetStorageBuffersDynamic << std::endl;
    out << "Max descriptor set storage images                        "
        << limits.maxDescriptorSetStorageImages << std::endl;
    out << "Max descriptor set uniform buffers                       "
        << limits.maxDescriptorSetUniformBuffers << std::endl;
    out << "Max descriptor set uniform buffers dynamic               "
        << limits.maxDescriptorSetUniformBuffersDynamic << std::endl;
    out << "Max draw indexed index value                             "
        << limits.maxDrawIndexedIndexValue << std::endl;
    out << "Max draw indirect count                                  "
        << limits.maxDrawIndirectCount << std::endl;
    out << "Max fragment combined output resources                   "
        << limits.maxFragmentCombinedOutputResources << std::endl;
    out << "Max fragment dual source attachments                     "
        << limits.maxFragmentDualSrcAttachments << std::endl;
    out << "Max fragment input components                            "
        << limits.maxFragmentInputComponents << std::endl;
    out << "Max fragment output components                           "
        << limits.maxFragmentOutputAttachments << std::endl;
    out << "Max framebuffer height                                   "
        << limits.maxFramebufferHeight << std::endl;
    out << "Max framebuffer layers                                   "
        << limits.maxFramebufferLayers << std::endl;
    out << "Max framebuffer width                                    "
        << limits.maxFramebufferWidth << std::endl;
    out << "Max geometry input components                            "
        << limits.maxGeometryInputComponents << std::endl;
    out << "Max geometry output components                           "
        << limits.maxGeometryOutputComponents << std::endl;
    out << "Max geometry output vertices                             "
        << limits.maxGeometryOutputVertices << std::endl;
    out << "Max geometry shader invocations                          "
        << limits.maxGeometryShaderInvocations << std::endl;
    out << "Max geometry total output components                     "
        << limits.maxGeometryTotalOutputComponents << std::endl;
    out << "Max image array layers                                   "
        << limits.maxImageArrayLayers << std::endl;
    out << "Max image dimension 1D                                   "
        << limits.maxImageDimension1D << std::endl;
    out << "Max image dimension 2D                                   "
        << limits.maxImageDimension2D << std::endl;
    out << "Max image dimension 3D                                   "
        << limits.maxImageDimension3D << std::endl;
    out << "Max image dimension cube                                 "
        << limits.maxImageDimensionCube << std::endl;
    out << "Max interpolation offset                                 "
        << limits.maxInterpolationOffset << std::endl;
    out << "Max memory allocation count                              "
        << limits.maxMemoryAllocationCount << std::endl;
    out << "Max per stage descriptor input attachments               "
        << limits.maxPerStageDescriptorInputAttachments << std::endl;
    out << "Max per stage descriptor sampled images                  "
        << limits.maxPerStageDescriptorSampledImages << std::endl;
    out << "Max per stage descriptor samplers                        "
        << limits.maxPerStageDescriptorSamplers << std::endl;
    out << "Max per stage descriptor storage buffers                 "
        << limits.maxPerStageDescriptorStorageBuffers << std::endl;
    out << "Max per stage descriptor storage images                  "
        << limits.maxPerStageDescriptorStorageImages << std::endl;
    out << "Max per stage descriptor uniform buffers                 "
        << limits.maxPerStageDescriptorUniformBuffers << std::endl;
    out << "Max per stage resources                                  "
        << limits.maxPerStageResources << std::endl;
    out << "Max push constants size                                  "
        << limits.maxPushConstantsSize << std::endl;
    out << "Max sample mask words                                    "
        << limits.maxSampleMaskWords << std::endl;
    out << "Max sampler allocation count                             "
        << limits.maxSamplerAllocationCount << std::endl;
    out << "Max sampler anisotropy                                   "
        << limits.maxSamplerAnisotropy << std::endl;
    out << "Max sampler LOD bias                                     "
        << limits.maxSamplerLodBias << std::endl;
    out << "Max storage buffer range                                 "
        << limits.maxStorageBufferRange << std::endl;
    out << "Max tesselation control per patch output components      "
        << limits.maxTessellationControlPerPatchOutputComponents << std::endl;
    out << "Max tesselation control per vertex input components      "
        << limits.maxTessellationControlPerVertexInputComponents << std::endl;
    out << "Max tesselation control per vertex output components     "
        << limits.maxTessellationControlPerVertexOutputComponents << std::endl;
    out << "Max tesselation control per total output components      "
        << limits.maxTessellationControlTotalOutputComponents << std::endl;
    out << "Max tesselation evaluation input components              "
        << limits.maxTessellationEvaluationInputComponents << std::endl;
    out << "Max tesselation evaluation output components             "
        << limits.maxTessellationEvaluationOutputComponents << std::endl;
    out << "Max tesselation generation level                         "
        << limits.maxTessellationGenerationLevel << std::endl;
    out << "Max tesselation generation patch size                    "
        << limits.maxTessellationPatchSize << std::endl;
    out << "Max texel buffer elements                                "
        << limits.maxTexelBufferElements << std::endl;
    out << "Max texel gather offset                                  "
        << limits.maxTexelGatherOffset << std::endl;
    out << "Max texel offset                                         "
        << limits.maxTexelOffset << std::endl;
    out << "Max uniform buffer range                                 "
        << limits.maxUniformBufferRange << std::endl;
    out << "Max vertex input attribute offset                        "
        << limits.maxVertexInputAttributeOffset << std::endl;
    out << "Max vertex input attributes                              "
        << limits.maxVertexInputAttributes << std::endl;
    out << "Max vertex input bindings                                "
        << limits.maxVertexInputBindings << std::endl;
    out << "Max vertex input binding stride                          "
        << limits.maxVertexInputBindingStride << std::endl;
    out << "Max vertex output components                             "
        << limits.maxVertexOutputComponents << std::endl;
    out << "Max viewport dimension X                                 "
        << limits.maxViewportDimensions[0] << std::endl;
    out << "Max viewport dimension Y                                 "
        << limits.maxViewportDimensions[1] << std::endl;
    out << "Max viewports                                            "
        << limits.maxViewports << std::endl;
    out << "Min interpolation offset                                 "
        << limits.minInterpolationOffset << std::endl;
    out << "Min memory map alignment                                 "
        << limits.minMemoryMapAlignment << std::endl;
    out << "Min storage buffer offset alignment                      "
        << limits.minStorageBufferOffsetAlignment << std::endl;
    out << "Min texel buffer offset alignment                        "
        << limits.minTexelBufferOffsetAlignment << std::endl;
    out << "Min texel gather offset                                  "
        << limits.minTexelGatherOffset << std::endl;
    out << "Min texel offset                                         "
        << limits.minTexelOffset << std::endl;
    out << "Min uniform buffer offset alignment                      "
        << limits.minUniformBufferOffsetAlignment << std::endl;
    out << "Mipmap precision bits                                    "
        << limits.mipmapPrecisionBits << std::endl;
    out << "Non coherent atom size                                   "
        << limits.nonCoherentAtomSize << std::endl;
    out << "Optimal buffer copy offset alignment                     "
        << limits.optimalBufferCopyOffsetAlignment << std::endl;
    out << "Optimal buffer copy row pitch alignment                  "
        << limits.optimalBufferCopyRowPitchAlignment << std::endl;
    out << "Point size granularity                                   "
        << limits.pointSizeGranularity << std::endl;
    out << "Point size min range                                     "
        << limits.pointSizeRange[0] << std::endl;
    out << "Point size max range                                     "
        << limits.pointSizeRange[1] << std::endl;
    out << "Sampled image color sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageColorSampleCounts);
    out << "Sampled image depth sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageDepthSampleCounts);
    out << "Sampled image integer sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageIntegerSampleCounts);
    out << "Sampled image stencil sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.sampledImageStencilSampleCounts);
    out << "Sparse address space size                                "
        << limits.sparseAddressSpaceSize << std::endl;
    out << "Standard sample locations                                "
        << static_cast<bool>(limits.standardSampleLocations) << std::endl;
    out << "Standard image sample counts:" << std::endl;
    DisplaySampleCountFlags(limits.storageImageSampleCounts);
    out << "Strict lines                                             "
        << static_cast<bool>(limits.strictLines) << std::endl;
    out << "Sub pixel interpolation offset bits                      "
        << limits.subPixelInterpolationOffsetBits << std::endl;
    out << "Sub pixel precision bits                                 "
        << limits.subPixelPrecisionBits << std::endl;
    out << "Sub texel precision bits                                 "
        << limits.subTexelPrecisionBits << std::endl;
    out << "Timestamp compute and graphics                           "
        << static_cast<bool>(limits.timestampComputeAndGraphics) << std::endl;
    out << "Timestamp period                                         "
        << limits.timestampPeriod << std::endl;
    out << "Viewport bounds min range                                "
        << limits.viewportBoundsRange[0] << std::endl;
    out << "Viewport bounds max range                                "
        << limits.viewportBoundsRange[1] << std::endl;
    out << "Viewport sub pixel bits                                  "
        << limits.viewportSubPixelBits << std::endl;
    out << std::noboolalpha;

    return out;
}
