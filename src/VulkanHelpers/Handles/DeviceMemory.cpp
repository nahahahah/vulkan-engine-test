#include "VulkanHelpers/Handles/DeviceMemory.hpp"

DeviceMemory::DeviceMemory(VkMemoryAllocateInfo const& allocateInfo, Device const& device, std::string const& label)
    : _label(label), _device(&device) {
    VkResult result = vkAllocateMemory(_device->Handle(), &allocateInfo, VK_NULL_HANDLE, &_handle);
    if (result != VK_SUCCESS) {
        std::string error = "Could not allocate \"" + _label + "\" memory (result: code " + std::to_string(result) + ")";
        throw std::runtime_error(error);
    }
    std::clog << "\"" << _label << "\" memory allocated successfully: <VkDeviceMemory " << _handle << ">" << std::endl;
}

DeviceMemory::DeviceMemory(DeviceMemory&& other) {
    _label = other._label;
    other._label = "";
    
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;
}

DeviceMemory::~DeviceMemory() {
    if (_handle != VK_NULL_HANDLE) {
        vkFreeMemory(_device->Handle(), _handle, VK_NULL_HANDLE);
        std::clog << "\"" << _label << "\" memory freed successfully" << std::endl;
        _handle = VK_NULL_HANDLE;
    }
}

DeviceMemory& DeviceMemory::operator = (DeviceMemory&& other) {
    _label = other._label;
    other._label = "";

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;

    _device = other._device;
    other._device = nullptr;

    return *this;
}
