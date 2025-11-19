#include "VulkanHelpers/ExtensionSupportCheck.hpp"

void DisplaySupportedExtensionsFromSDLVulkan(std::vector<const char*> const& enabledExtensions, std::vector<ExtensionProperties> const& supportedExtensions) {
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
