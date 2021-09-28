#include "mvBuffer.h"
#include <stdexcept>
#include "mvContext.h"

namespace DearPy3D {

    mvBuffer mvCreateBuffer(void* data, uint64_t count, uint64_t size, VkBufferUsageFlags flags)
    {
        mvBuffer buffer{};
        buffer.count = count;

        VkDeviceSize bufferSize = size * count;
        buffer.size = (size_t)bufferSize;

        // create staging buffer to copy data to
        VkBufferCreateInfo stagingBufferInfo{};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.size = bufferSize;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkBuffer stagingBuffer;
        VmaAllocation stagingBufferAllocation = mvAllocateBuffer(stagingBufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);
        void* mdata = mvMapMemory(stagingBufferAllocation);
        memcpy(mdata, data, buffer.size);
        mvUnmapMemory(stagingBufferAllocation);

        // create final buffer
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | flags;
        buffer.memoryAllocation = mvAllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_GPU_ONLY, buffer.buffer);

        // copy from staging to final buffer
        VkBufferCopy copyRegion = {};
        copyRegion.size = bufferSize;
        VkCommandBuffer copyCmd = mvBeginSingleTimeCommands();
        vkCmdCopyBuffer(copyCmd, stagingBuffer, buffer.buffer, 1, &copyRegion);
        mvEndSingleTimeCommands(copyCmd);

        // cleanup staging buffer
        mvDestroyBuffer(stagingBuffer, stagingBufferAllocation);

        return buffer;
    }

    mvBuffer mvCreateDynamicBuffer(void* data, uint64_t count, uint64_t size, VkBufferUsageFlags flags)
    {
        size_t bufferAlignment = mvGetRequiredUniformBufferSize(size);

        mvBuffer buffer{};
        buffer.count = count;

        VkDeviceSize bufferSize = bufferAlignment * count;
        buffer.size = (size_t)bufferSize;

        // create staging buffer to copy data to
        VkBufferCreateInfo stagingBufferInfo{};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.size = bufferSize;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkBuffer stagingBuffer;
        VmaAllocation stagingBufferAllocation = mvAllocateBuffer(stagingBufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);
        char* mdata = (char*)mvMapMemory(stagingBufferAllocation);
        char* sdata = (char*)data;
        
        uint64_t currentOffset = 0u;
        for (size_t i = 0; i < count; i++)
        {
            char* dst = &mdata[i * bufferAlignment];
            char* src = &sdata[i * size];
            memcpy(dst, src, size);
        }
        mvUnmapMemory(stagingBufferAllocation);

        // create final buffer
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | flags;
        buffer.memoryAllocation = mvAllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, buffer.buffer);

        // copy from staging to final buffer
        VkBufferCopy copyRegion = {};
        copyRegion.size = bufferSize;
        VkCommandBuffer copyCmd = mvBeginSingleTimeCommands();
        vkCmdCopyBuffer(copyCmd, stagingBuffer, buffer.buffer, 1, &copyRegion);
        mvEndSingleTimeCommands(copyCmd);

        // cleanup staging buffer
        mvDestroyBuffer(stagingBuffer, stagingBufferAllocation);

        return buffer;
    }

    
    void mvCleanupBuffer(mvBuffer& buffer)
    {
        vkDestroyBuffer(mvGetLogicalDevice(), buffer.buffer, nullptr);
        mvFree(buffer.memoryAllocation);
    }

    void mvUpdateBuffer(mvBuffer& buffer, void* data)
    {
        void* mdata = mvMapMemory(buffer.memoryAllocation);
        memcpy(mdata, data, buffer.size);
        mvUnmapMemory(buffer.memoryAllocation);
    }
}
