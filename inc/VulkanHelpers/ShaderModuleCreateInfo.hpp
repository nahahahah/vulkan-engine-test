#ifndef VK_WRAPPER_SHADER_MODULE_CREATE_INFO_HPP
#define VK_WRAPPER_SHADER_MODULE_CREATE_INFO_HPP

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

VkShaderModuleCreateInfo GenerateShaderModuleCreateInfo(std::filesystem::path const& shaderPath, std::vector<char>& shaderCode);

#endif // VK_WRAPPER_SHADER_MODULE_INFO_HPP