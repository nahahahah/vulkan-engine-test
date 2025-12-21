#include "VulkanHelpers/Handles/DescriptorSet.hpp"

DescriptorSetCollection::DescriptorSetCollection(
    VkDescriptorSetAllocateInfo const& allocateInfo,
    Device const& device,
    DescriptorPool const* descriptorPool
) : _device(&device), _descriptorPool(descriptorPool) {
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

DescriptorSetCollection::DescriptorSetCollection(DescriptorSetCollection&& other) {
    _handles = other._handles;
    other._handles = {};

    _device = other._device;
    other._device = nullptr;

    _descriptorPool = other._descriptorPool;
    other._descriptorPool = nullptr;
}

DescriptorSetCollection::~DescriptorSetCollection() {
    /*
    // add this case when the pool allows it
    vkFreeDescriptorSetCollection(
        _device->Handle(),
        _descriptorPool->Handle(),
        static_cast<uint32_t>(_handles.size()),
        _handles.data()
    );
    */
}

DescriptorSetCollection& DescriptorSetCollection::operator = (DescriptorSetCollection&& other) {
    _handles = other._handles;
    other._handles = {};

    _device = other._device;
    other._device = nullptr;

    _descriptorPool = other._descriptorPool;
    other._descriptorPool = nullptr;

    return *this;
}

VkDescriptorSet& DescriptorSetCollection::operator [] (size_t index) {
    assert("`index` must be within the bounds of the container" &&
           index < _handles.size());

    return _handles[index];
}

VkDescriptorSet const& DescriptorSetCollection::operator [] (size_t index) const {
    assert("`index` must be within the bounds of the container" &&
           index < _handles.size());

    return _handles[index];
}
