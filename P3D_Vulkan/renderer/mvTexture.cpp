#include "mvTexture.h"
#include <stdexcept>
#include "mvContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace DearPy3D {

    mvTexture mvCreateTexture(const std::string& file)
    {
        mvTexture texture{};

        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(file.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels)
            throw std::runtime_error("failed to load texture image!");

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        mvCreateBuffer(imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(mvGetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(mvGetLogicalDevice(), stagingBufferMemory);

        stbi_image_free(pixels);

        mvCreateImage(texWidth, texHeight,
            VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            texture.textureImage, texture.textureImageMemory);

        mvTransitionImageLayout(texture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        mvCopyBufferToImage(stagingBuffer, texture.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        mvTransitionImageLayout(texture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(mvGetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(mvGetLogicalDevice(), stagingBufferMemory, nullptr);

        texture.textureImageView = mvCreateImageView(texture.textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);

        return texture;
    }

    void mvCleanupTexture(mvTexture& texture)
    {
        vkDestroyImageView(mvGetLogicalDevice(), texture.textureImageView, nullptr);
        vkDestroyImage(mvGetLogicalDevice(), texture.textureImage, nullptr);
        vkFreeMemory(mvGetLogicalDevice(), texture.textureImageMemory, nullptr);
    }

}