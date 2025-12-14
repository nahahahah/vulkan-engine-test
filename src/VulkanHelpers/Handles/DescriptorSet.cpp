#include "VulkanHelpers/Handles/DescriptorSet.hpp"

DescriptorSets::DescriptorSets(VkDescriptorSetAllocateInfo const& allocateInfo, Device& device, DescriptorPool& descriptorPool)
    : _device(&device), _descriptorPool(&descriptorPool) {
        _handles.resize(allocateInfo.descriptorSetCount);
    VkResult result = vkAllocateDescriptorSets(_device->Handle(), &allocateInfo, _handles.data());
    if (result != VK_SUCCESS) {
        std::string error = "Could not create descriptor sets (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "Descriptor sets created successfully:" << std::endl;

    for (auto& handle : _handles) {
        std::clog << " - <VkDescriptorSet " << handle << ">" << std::endl;
    }
}

DescriptorSets::~DescriptorSets() {
    /*
    // add this case when the pool allows it
    vkFreeDescriptorSets(
        _device->Handle(),
        _descriptorPool->Handle(),
        static_cast<uint32_t>(_handles.size()),
        _handles.data()
    );
    */
}
