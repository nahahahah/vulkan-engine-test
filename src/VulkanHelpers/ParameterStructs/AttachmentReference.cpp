#include "VulkanHelpers/ParameterStructs/AttachmentReference.hpp"

VkAttachmentReference2 GenerateAttachmentReference(uint32_t attachment) {
    VkAttachmentReference2 reference {};

    // structure type
    reference.sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2;

    // attachement properties
    reference.attachment = attachment;
    reference.aspectMask = VK_IMAGE_ASPECT_NONE;
    reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // extend reference
    reference.pNext = VK_NULL_HANDLE;

    return reference;
}
