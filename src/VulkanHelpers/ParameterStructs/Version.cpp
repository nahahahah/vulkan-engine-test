#include "VulkanHelpers/ParameterStructs/Version.hpp"

Version::Version(uint32_t version)
    : variant(VK_API_VERSION_VARIANT(version))
    ,   major(  VK_API_VERSION_MAJOR(version))
    ,   minor(  VK_API_VERSION_MINOR(version))
    ,   patch(  VK_API_VERSION_PATCH(version)) {}

std::ostream& operator << (std::ostream& out, Version const& v) {
    return (out << v.variant << "." << v.major << "." << v.minor << "." << v.patch);
}

DriverVersion::DriverVersion(uint32_t version, uint32_t vendorID) {
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
