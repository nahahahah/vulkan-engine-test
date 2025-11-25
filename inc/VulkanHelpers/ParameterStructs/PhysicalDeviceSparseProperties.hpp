#ifndef VK_WRAPPER_PHYSICAL_DEVICE_SPARSE_PROPERTIES_HPP
#define VK_WRAPPER_PHYSICAL_DEVICE_SPARSE_PROPERTIES_HPP

#include <iostream>

#include <vulkan/vulkan.h>

std::ostream& operator << (std::ostream& out, VkPhysicalDeviceSparseProperties const& sparseProperties);

#endif // VK_WRAPPER_PHYSICAL_DEVICE_SPARSE_PROPERTIES_HPP
