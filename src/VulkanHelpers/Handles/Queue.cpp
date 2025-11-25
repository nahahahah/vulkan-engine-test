#include "VulkanHelpers/Handles/Queue.hpp"

Queue::Queue(VkDeviceQueueInfo2 const& queueInfo, Device const& device) {
    vkGetDeviceQueue2(device.Handle(), &queueInfo, &_handle);
}
