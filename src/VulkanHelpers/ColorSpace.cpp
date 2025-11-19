#include "VulkanHelpers/ColorSpace.hpp"

std::ostream& operator << (std::ostream& out, VkColorSpaceKHR const& colorSpace) {
    out << "\t\t\t - Color space     ";
    switch (colorSpace) {
        case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: {
			out << "COLOR_SPACE_SRGB_NONLINEAR_KHR";
			break;
		}

        case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT: {
			out << "COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT: {
			out << "COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT: {
			out << "COLOR_SPACE_DISPLAY_P3_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT: {
			out << "COLOR_SPACE_DCI_P3_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_BT709_LINEAR_EXT: {
			out << "COLOR_SPACE_BT709_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_BT709_NONLINEAR_EXT: {
			out << "COLOR_SPACE_BT709_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_BT2020_LINEAR_EXT: {
			out << "COLOR_SPACE_BT2020_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_HDR10_ST2084_EXT: {
			out << "COLOR_SPACE_HDR10_ST2084_EXT";
			break;
		}

        case VK_COLOR_SPACE_DOLBYVISION_EXT: {
			out << "COLOR_SPACE_DOLBYVISION_EXT";
			break;
		}

        case VK_COLOR_SPACE_HDR10_HLG_EXT: {
			out << "COLOR_SPACE_HDR10_HLG_EXT";
			break;
		}

        case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT: {
			out << "COLOR_SPACE_ADOBERGB_LINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT: {
			out << "COLOR_SPACE_ADOBERGB_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_PASS_THROUGH_EXT: {
			out << "COLOR_SPACE_PASS_THROUGH_EXT";
			break;
		}

        case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: {
			out << "COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT";
			break;
		}

        case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD: {
			out << "COLOR_SPACE_DISPLAY_NATIVE_AMD";
			break;
		}

        default: {
            out << "Unknown (" << colorSpace << ")";
            break;
        }
    }
    out << std::endl;

	return out;
}
