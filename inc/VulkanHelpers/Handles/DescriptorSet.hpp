#ifndef VK_WRAPPER_DESCRIPTOR_SET_HPP
#define VK_WRAPPER_DESCRIPTOR_SET_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"
#include "VulkanHelpers/Handles/DescriptorPool.hpp"

class DescriptorSets {
    public:
        DescriptorSets(VkDescriptorSetAllocateInfo const& allocateInfo, Device& device, DescriptorPool& descriptorPool);
        ~DescriptorSets();

        VkDescriptorSet* Handles() { return _handles.data(); }
        VkDescriptorSet const* Handles() const { return _handles.data(); }

        std::vector<VkDescriptorSet>& HandlesContainer() { return _handles; }
        std::vector<VkDescriptorSet> const& HandlesContainer() const { return _handles; }

    private:
        std::vector<VkDescriptorSet> _handles {};
        Device* _device = nullptr;
        DescriptorPool* _descriptorPool = nullptr;
};

#endif // VK_WRAPPER_DESCRIPTOR_SET_HPP