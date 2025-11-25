#include "VulkanHelpers/ParameterStructs/AttachmentDescription.hpp"

VkAttachmentDescription2 GenerateAttachmentDescription(VkFormat format) {
    VkAttachmentDescription2 description {};

    // structure type and flags
    description.sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2;
    description.flags = 0;

    // operations properties
    description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    // layout prperties
    description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // samples properties
    description.samples = VK_SAMPLE_COUNT_1_BIT;
    description.format = format;

    // extend description
    description.pNext = VK_NULL_HANDLE;

    return description;
}