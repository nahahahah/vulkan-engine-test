#include "VulkanHelpers/ParameterStructs/PhysicalDeviceSparseProperties.hpp"

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceSparseProperties const& sparseProperties) {
    out << std::boolalpha;
    out << " - Residency aligned mip size                        "
        << static_cast<bool>(sparseProperties.residencyAlignedMipSize) << std::endl;
    out << " - Residency non resident strict                     "
        << static_cast<bool>(sparseProperties.residencyNonResidentStrict) << std::endl;
    out << " - Residency standard 2D block shape                 "
        << static_cast<bool>(sparseProperties.residencyStandard2DBlockShape) << std::endl;
    out << " - Residency standard 2D multisample block shape     "
        << static_cast<bool>(sparseProperties.residencyStandard2DMultisampleBlockShape) << std::endl;
    out << " - Residency standard 3D block shape                 "
        << static_cast<bool>(sparseProperties.residencyStandard3DBlockShape) << std::endl;
    out << std::noboolalpha;

    return out;
}
