#include "mvAllocator.h"
#include "mvContext.h"

static VmaAllocator gallocator;

namespace DearPy3D {

    void mvInitializeAllocator()
    {   
        // Initialize VulkanMemoryAllocator
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = 0;
        allocatorInfo.physicalDevice = mvGetPhysicalDevice();
        allocatorInfo.device = mvGetLogicalDevice();
        allocatorInfo.instance = mvGetVkInstance();

        vmaCreateAllocator(&allocatorInfo, &gallocator);
    }

    void mvShutdownAllocator()
    {
        vmaDestroyAllocator(gallocator);
    }

    VmaAllocation mvAllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer)
    {
        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = usage;

        VmaAllocation allocation;
        vmaCreateBuffer(gallocator, &bufferCreateInfo, &allocCreateInfo, &outBuffer, &allocation, nullptr);

        //VmaAllocationInfo allocInfo{};
        //vmaGetAllocationInfo(GetVmaAllocator(), allocation, &allocInfo);

        return allocation;
    }

    VmaAllocation mvAllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage)
    {
        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = usage;

        VmaAllocation allocation;
        vmaCreateImage(gallocator, &imageCreateInfo, &allocCreateInfo, &outImage, &allocation, nullptr);

        //VmaAllocationInfo allocInfo;
        //vmaGetAllocationInfo(GetVmaAllocator(), allocation, &allocInfo);

        return allocation;
    }

    void mvFree(VmaAllocation allocation)
    {
        vmaFreeMemory(gallocator, allocation);
    }

    void mvDestroyImage(VkImage image, VmaAllocation allocation)
    {
        vmaDestroyImage(gallocator, image, allocation);
    }

    void mvDestroyBuffer(VkBuffer buffer, VmaAllocation allocation)
    {
        vmaDestroyBuffer(gallocator, buffer, allocation);
    }

    void* mvMapMemory(VmaAllocation allocation)
    {
        void* mappedMemory;
        vmaMapMemory(gallocator, allocation, (void**)&mappedMemory);
        return mappedMemory;
    }
    
    void mvUnmapMemory(VmaAllocation allocation)
    {
        vmaUnmapMemory(gallocator, allocation);
    }

}
