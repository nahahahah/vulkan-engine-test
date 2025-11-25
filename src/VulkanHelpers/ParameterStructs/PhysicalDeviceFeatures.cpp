#include "VulkanHelpers/ParameterStructs/PhysicalDeviceFeatures.hpp"

VkPhysicalDeviceFeatures2 GeneratePhysicalDeviceFeatures(PhysicalDevice const& physicalDevice) {
    VkPhysicalDeviceFeatures2 features {};

    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features.features = {};
    features.pNext = VK_NULL_HANDLE;

    vkGetPhysicalDeviceFeatures2(physicalDevice.Handle(), &features);

    return features;
}

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceFeatures2 const& physicalDeviceFeatures) {
    out << std::boolalpha;
    out << " - Features:" << std::endl;
    out << " - Alpha to one                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.alphaToOne) << std::endl;
    out << " - Depth bias clamp                                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.depthBiasClamp) << std::endl;
    out << " - Depth bounds                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.depthBounds) << std::endl;
    out << " - Depth clamp                                      " 
        << static_cast<bool>(physicalDeviceFeatures.features.depthClamp) << std::endl;
    out << " - Draw indirect first instance                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.drawIndirectFirstInstance) << std::endl;
    out << " - Dual source blend                                " 
        << static_cast<bool>(physicalDeviceFeatures.features.dualSrcBlend) << std::endl;
    out << " - Fill mode non solid                              " 
        << static_cast<bool>(physicalDeviceFeatures.features.fillModeNonSolid) << std::endl;
    out << " - Fragment stores and atomics                      " 
        << static_cast<bool>(physicalDeviceFeatures.features.fragmentStoresAndAtomics) << std::endl;
    out << " - Full draw index Uint32                           " 
        << static_cast<bool>(physicalDeviceFeatures.features.fullDrawIndexUint32) << std::endl;
    out << " - Geometry shader                                  " 
        << static_cast<bool>(physicalDeviceFeatures.features.geometryShader) << std::endl;
    out << " - Image cube array                                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.imageCubeArray) << std::endl;
    out << " - Independent blend                                " 
        << static_cast<bool>(physicalDeviceFeatures.features.independentBlend) << std::endl;
    out << " - Inherited queries                                " 
        << static_cast<bool>(physicalDeviceFeatures.features.inheritedQueries) << std::endl;
    out << " - Large points                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.largePoints) << std::endl;
    out << " - Logic operations                                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.logicOp) << std::endl;
    out << " - Multi draw indirect                              " 
        << static_cast<bool>(physicalDeviceFeatures.features.multiDrawIndirect) << std::endl;
    out << " - Multi viewport                                   " 
        << static_cast<bool>(physicalDeviceFeatures.features.multiViewport) << std::endl;
    out << " - Occlusion query precise                          " 
        << static_cast<bool>(physicalDeviceFeatures.features.occlusionQueryPrecise) << std::endl;
    out << " - Pipeline statistics query                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.pipelineStatisticsQuery) << std::endl;
    out << " - Robust buffer access                             " 
        << static_cast<bool>(physicalDeviceFeatures.features.robustBufferAccess) << std::endl;
    out << " - Sampler anisotropy                               " 
        << static_cast<bool>(physicalDeviceFeatures.features.samplerAnisotropy) << std::endl;
    out << " - Sample rate shading                              " 
        << static_cast<bool>(physicalDeviceFeatures.features.sampleRateShading) << std::endl;
    out << " - Shader clip distance                             " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderClipDistance) << std::endl;
    out << " - Shader cull distance                             " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderCullDistance) << std::endl;
    out << " - Shader Float64                                   " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderFloat64) << std::endl;
    out << " - Shader image gather extended                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderImageGatherExtended) << std::endl;
    out << " - Shader Int16                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderInt16) << std::endl;
    out << " - Shader Int64                                     "  
        << static_cast<bool>(physicalDeviceFeatures.features.shaderInt64) << std::endl;
    out << " - Shader resource min LOD                          " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderResourceMinLod) << std::endl;
    out << " - Shader resource residency                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderResourceResidency) << std::endl;
    out << " - Shader sampled image array dynamic indexing      " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderSampledImageArrayDynamicIndexing) << std::endl;
    out << " - Shader storage buffer array dynamic indexing     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageBufferArrayDynamicIndexing) << std::endl;
    out << " - Shader storage image array dynamic indexing      " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageArrayDynamicIndexing) << std::endl;
    out << " - Shader storage image extended formats            " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageExtendedFormats) << std::endl;
    out << " - Shader storage image multisample                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageMultisample) << std::endl;
    out << " - Shader storage image read without format         " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageReadWithoutFormat) << std::endl;
    out << " - Shader storage image write withou format         " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageWriteWithoutFormat) << std::endl;
    out << " - Shader tesselation and geometry point size       " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderTessellationAndGeometryPointSize) << std::endl;
    out << " - Shader uniform buffer array dynamic indexing     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderUniformBufferArrayDynamicIndexing) << std::endl;
    out << " - Sparse binding                                   " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseBinding) << std::endl;
    out << " - Sparse residency 16 samples                      " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency16Samples) << std::endl;
    out << " - Sparse residency 2 samples                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency2Samples) << std::endl;
    out << " - Sparse residency 4 samples                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency4Samples) << std::endl;
    out << " - Sparse residency 8 samples                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency8Samples) << std::endl;
    out << " - Sparse residency aliased                         " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyAliased) << std::endl;
    out << " - Sparse residency buffer                          " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyBuffer) << std::endl;
    out << " - Sparse residency image 2D                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyImage2D) << std::endl;
    out << " - Sparse residency image 3D                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyImage3D) << std::endl;
    out << " - Tesselation shader                               " 
        << static_cast<bool>(physicalDeviceFeatures.features.tessellationShader) << std::endl;
    out << " - Texture compression ASTC_LDR                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionASTC_LDR) << std::endl;
    out << " - Texture compression BC                           " 
        << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionBC) << std::endl;
    out << " - Texture compression ETC2                         " 
        << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionETC2) << std::endl;
    out << " - Variable multisample rate                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.variableMultisampleRate) << std::endl;
    out << " - Vertex pipeline stores and atomics               " 
        << static_cast<bool>(physicalDeviceFeatures.features.vertexPipelineStoresAndAtomics) << std::endl;
    out << " - Wide lines                                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.wideLines) << std::endl;
    out << std::noboolalpha;

    return out;
}
