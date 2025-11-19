#ifndef VK_WRAPPER_EXTENSION_PROPERTIES_HPP
#define VK_WRAPPER_EXTENSION_PROPERTIES_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <span>
#include <string>

#include <vulkan/vulkan.h>

struct ExtensionProperties : public VkExtensionProperties {
    public:
        ExtensionProperties() = default;

        static std::vector<ExtensionProperties> Enumerate();

        friend std::ostream& operator << (std::ostream& out, ExtensionProperties const& extensionProperties);
        friend std::ostream& operator << (std::ostream& out, std::span<ExtensionProperties> extensionsProperties);
};

#endif // VK_WRAPPER_EXTENSION_PROPERTIES_HPP