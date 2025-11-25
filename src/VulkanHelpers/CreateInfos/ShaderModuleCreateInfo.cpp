#include "VulkanHelpers/CreateInfos/ShaderModuleCreateInfo.hpp"

VkShaderModuleCreateInfo GenerateShaderModuleCreateInfo(std::filesystem::path const& shaderPath, std::vector<char>& shaderFileBuffer) {
    std::ifstream shaderFile(shaderPath, std::ios::ate | std::ios::binary);
    if (!shaderFile.is_open()) {
        std::string error = "Unable to open the shader file at " + shaderPath.string();
        throw std::runtime_error(error);
    }
    std::clog << "Vertex shader file opened successfully" << std::endl;

    size_t shaderFileSize = static_cast<size_t>(shaderFile.tellg());
    
    // clean file buffer and reset its content with the shader content
    shaderFileBuffer.clear();
    shaderFileBuffer.resize(shaderFileSize);
    shaderFile.seekg(0);
    shaderFile.read(shaderFileBuffer.data(), shaderFileSize);

    shaderFile.close();

    VkShaderModuleCreateInfo createInfo {};

    // structure type and flags
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.flags = 0;

    // shader properties
    createInfo.codeSize = shaderFileBuffer.size();
    createInfo.pCode = reinterpret_cast<uint32_t const*>(shaderFileBuffer.data());
    
    // extend create info
    createInfo.pNext = VK_NULL_HANDLE;

    return createInfo;
}
