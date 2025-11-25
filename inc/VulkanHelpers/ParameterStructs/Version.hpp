#ifndef VK_WRAPPER_VERSION_HPP
#define VK_WRAPPER_VERSION_HPP

#include <iostream>

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
    Version(uint32_t version);

    friend std::ostream& operator << (std::ostream& out, Version const& v);

    int variant = 0;
    int major = 0;
    int minor = 0;
    int patch = 0;
};

struct DriverVersion : public Version {
    DriverVersion() = default;
    DriverVersion(uint32_t version, uint32_t vendorID);
};

#endif // VK_WRAPPER_VERSION_HPP
