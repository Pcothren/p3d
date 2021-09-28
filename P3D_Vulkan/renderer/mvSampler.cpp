#include "mvSampler.h"
#include <stdexcept>
#include "mvContext.h"

namespace DearPy3D {

    mvSampler mvCreateSampler()
	{

        mvSampler sampler{};

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(mvGetPhysicalDevice(), &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(mvGetLogicalDevice(), &samplerInfo, nullptr, &sampler.textureSampler) != VK_SUCCESS)
            throw std::runtime_error("failed to create texture sampler!");

        return sampler;
	}

    void mvCleanupSampler(mvSampler& sampler)
    {
        vkDestroySampler(mvGetLogicalDevice(), sampler.textureSampler, nullptr);
        sampler.textureSampler = VK_NULL_HANDLE;
    }

}