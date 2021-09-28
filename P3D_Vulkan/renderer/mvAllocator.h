#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace DearPy3D {

    void mvInitializeAllocator();
    void mvShutdownAllocator();

    VmaAllocation mvAllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer);
    VmaAllocation mvAllocateImage (VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage);
    void          mvFree          (VmaAllocation allocation);
    void          mvDestroyImage  (VkImage image, VmaAllocation allocation);
    void          mvDestroyBuffer (VkBuffer buffer, VmaAllocation allocation);
    void*         mvMapMemory     (VmaAllocation allocation);
    void          mvUnmapMemory   (VmaAllocation allocation);

}
