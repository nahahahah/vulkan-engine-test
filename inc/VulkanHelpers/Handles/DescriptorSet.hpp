#ifndef VK_WRAPPER_DESCRIPTOR_SET_HPP
#define VK_WRAPPER_DESCRIPTOR_SET_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <span>
#include <vector>
#include <cassert>

#include <vulkan/vulkan.h>

#include "VulkanHelpers/Handles/Device.hpp"
#include "VulkanHelpers/Handles/DescriptorPool.hpp"

class DescriptorSetCollection {
    public:
        using Iterator = std::vector<VkDescriptorSet>::iterator;
        using ConstIterator = std::vector<VkDescriptorSet>::const_iterator;
        using ReverseIterator = std::vector<VkDescriptorSet>::reverse_iterator;
        using ConstReverseIterator = std::vector<VkDescriptorSet>::const_reverse_iterator;

        DescriptorSetCollection() = default;
        DescriptorSetCollection(VkDescriptorSetAllocateInfo const& allocateInfo, Device const& device, std::string const& label, DescriptorPool const* descriptorPool = nullptr);
        DescriptorSetCollection(DescriptorSetCollection const& other) = delete;
        DescriptorSetCollection(DescriptorSetCollection&& other);
        ~DescriptorSetCollection();

        DescriptorSetCollection& operator = (DescriptorSetCollection const& other) = delete;
        DescriptorSetCollection& operator = (DescriptorSetCollection&& other);

        VkDescriptorSet& operator [] (size_t index);
        VkDescriptorSet const& operator [] (size_t index) const;

        size_t size() { return _handles.size(); }

        std::string Label() { return _label; }
        std::string Label() const { return _label; }
        std::string Label(std::string const& label) { _label = label; }

        VkDescriptorSet* data() { return _handles.data(); }
        VkDescriptorSet const* data() const { return _handles.data(); }

        Iterator begin() { return _handles.begin(); }
        ConstIterator begin() const { return _handles.begin(); }
        
        Iterator end() { return _handles.end(); }
        ConstIterator end() const { return _handles.end(); }
        
        ConstIterator cbegin() const { return _handles.cbegin(); }
        ConstIterator cend() const { return _handles.cend(); }
        
        ReverseIterator rbegin() { return _handles.rbegin(); }
        ConstReverseIterator rbegin() const { return _handles.rbegin(); }
        
        ReverseIterator rend() { return _handles.rend(); }
        ConstReverseIterator rend() const{ return _handles.rend(); }
        
        ConstReverseIterator crbegin() const { return _handles.crbegin(); }
        ConstReverseIterator crend() const { return _handles.crend(); }

    private:
        std::string _label = "";
        std::vector<VkDescriptorSet> _handles {};
        Device const* _device = nullptr;
        DescriptorPool const* _descriptorPool = nullptr;
};

#endif // VK_WRAPPER_DESCRIPTOR_SET_HPP