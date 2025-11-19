#include "VulkanHelpers/PhysicalDeviceFeatures.hpp"

PhysicalDeviceFeatures::PhysicalDeviceFeatures(PhysicalDevice const& physicalDevice) {
    this->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    this->features = {};
    this->pNext = VK_NULL_HANDLE;

    vkGetPhysicalDeviceFeatures2(physicalDevice.Handle(), this);
}

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceFeatures2 const& physicalDeviceFeatures) {
    out << std::boolalpha;
    out << "\t - Features:" << std::endl;
    out << "\t\t - Alpha to one                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.alphaToOne) << std::endl;
    out << "\t\t - Depth bias clamp                                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.depthBiasClamp) << std::endl;
    out << "\t\t - Depth bounds                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.depthBounds) << std::endl;
    out << "\t\t - Depth clamp                                      " 
        << static_cast<bool>(physicalDeviceFeatures.features.depthClamp) << std::endl;
    out << "\t\t - Draw indirect first instance                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.drawIndirectFirstInstance) << std::endl;
    out << "\t\t - Dual source blend                                " 
        << static_cast<bool>(physicalDeviceFeatures.features.dualSrcBlend) << std::endl;
    out << "\t\t - Fill mode non solid                              " 
        << static_cast<bool>(physicalDeviceFeatures.features.fillModeNonSolid) << std::endl;
    out << "\t\t - Fragment stores and atomics                      " 
        << static_cast<bool>(physicalDeviceFeatures.features.fragmentStoresAndAtomics) << std::endl;
    out << "\t\t - Full draw index Uint32                           " 
        << static_cast<bool>(physicalDeviceFeatures.features.fullDrawIndexUint32) << std::endl;
    out << "\t\t - Geometry shader                                  " 
        << static_cast<bool>(physicalDeviceFeatures.features.geometryShader) << std::endl;
    out << "\t\t - Image cube array                                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.imageCubeArray) << std::endl;
    out << "\t\t - Independent blend                                " 
        << static_cast<bool>(physicalDeviceFeatures.features.independentBlend) << std::endl;
    out << "\t\t - Inherited queries                                " 
        << static_cast<bool>(physicalDeviceFeatures.features.inheritedQueries) << std::endl;
    out << "\t\t - Large points                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.largePoints) << std::endl;
    out << "\t\t - Logic operations                                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.logicOp) << std::endl;
    out << "\t\t - Multi draw indirect                              " 
        << static_cast<bool>(physicalDeviceFeatures.features.multiDrawIndirect) << std::endl;
    out << "\t\t - Multi viewport                                   " 
        << static_cast<bool>(physicalDeviceFeatures.features.multiViewport) << std::endl;
    out << "\t\t - Occlusion query precise                          " 
        << static_cast<bool>(physicalDeviceFeatures.features.occlusionQueryPrecise) << std::endl;
    out << "\t\t - Pipeline statistics query                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.pipelineStatisticsQuery) << std::endl;
    out << "\t\t - Robust buffer access                             " 
        << static_cast<bool>(physicalDeviceFeatures.features.robustBufferAccess) << std::endl;
    out << "\t\t - Sampler anisotropy                               " 
        << static_cast<bool>(physicalDeviceFeatures.features.samplerAnisotropy) << std::endl;
    out << "\t\t - Sample rate shading                              " 
        << static_cast<bool>(physicalDeviceFeatures.features.sampleRateShading) << std::endl;
    out << "\t\t - Shader clip distance                             " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderClipDistance) << std::endl;
    out << "\t\t - Shader cull distance                             " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderCullDistance) << std::endl;
    out << "\t\t - Shader Float64                                   " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderFloat64) << std::endl;
    out << "\t\t - Shader image gather extended                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderImageGatherExtended) << std::endl;
    out << "\t\t - Shader Int16                                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderInt16) << std::endl;
    out << "\t\t - Shader Int64                                     "  
        << static_cast<bool>(physicalDeviceFeatures.features.shaderInt64) << std::endl;
    out << "\t\t - Shader resource min LOD                          " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderResourceMinLod) << std::endl;
    out << "\t\t - Shader resource residency                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderResourceResidency) << std::endl;
    out << "\t\t - Shader sampled image array dynamic indexing      " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderSampledImageArrayDynamicIndexing) << std::endl;
    out << "\t\t - Shader storage buffer array dynamic indexing     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageBufferArrayDynamicIndexing) << std::endl;
    out << "\t\t - Shader storage image array dynamic indexing      " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageArrayDynamicIndexing) << std::endl;
    out << "\t\t - Shader storage image extended formats            " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageExtendedFormats) << std::endl;
    out << "\t\t - Shader storage image multisample                 " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageMultisample) << std::endl;
    out << "\t\t - Shader storage image read without format         " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageReadWithoutFormat) << std::endl;
    out << "\t\t - Shader storage image write withou format         " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderStorageImageWriteWithoutFormat) << std::endl;
    out << "\t\t - Shader tesselation and geometry point size       " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderTessellationAndGeometryPointSize) << std::endl;
    out << "\t\t - Shader uniform buffer array dynamic indexing     " 
        << static_cast<bool>(physicalDeviceFeatures.features.shaderUniformBufferArrayDynamicIndexing) << std::endl;
    out << "\t\t - Sparse binding                                   " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseBinding) << std::endl;
    out << "\t\t - Sparse residency 16 samples                      " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency16Samples) << std::endl;
    out << "\t\t - Sparse residency 2 samples                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency2Samples) << std::endl;
    out << "\t\t - Sparse residency 4 samples                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency4Samples) << std::endl;
    out << "\t\t - Sparse residency 8 samples                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidency8Samples) << std::endl;
    out << "\t\t - Sparse residency aliased                         " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyAliased) << std::endl;
    out << "\t\t - Sparse residency buffer                          " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyBuffer) << std::endl;
    out << "\t\t - Sparse residency image 2D                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyImage2D) << std::endl;
    out << "\t\t - Sparse residency image 3D                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.sparseResidencyImage3D) << std::endl;
    out << "\t\t - Tesselation shader                               " 
        << static_cast<bool>(physicalDeviceFeatures.features.tessellationShader) << std::endl;
    out << "\t\t - Texture compression ASTC_LDR                     " 
        << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionASTC_LDR) << std::endl;
    out << "\t\t - Texture compression BC                           " 
        << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionBC) << std::endl;
    out << "\t\t - Texture compression ETC2                         " 
        << static_cast<bool>(physicalDeviceFeatures.features.textureCompressionETC2) << std::endl;
    out << "\t\t - Variable multisample rate                        " 
        << static_cast<bool>(physicalDeviceFeatures.features.variableMultisampleRate) << std::endl;
    out << "\t\t - Vertex pipeline stores and atomics               " 
        << static_cast<bool>(physicalDeviceFeatures.features.vertexPipelineStoresAndAtomics) << std::endl;
    out << "\t\t - Wide lines                                       " 
        << static_cast<bool>(physicalDeviceFeatures.features.wideLines) << std::endl;
    out << std::noboolalpha;

    return out;
}