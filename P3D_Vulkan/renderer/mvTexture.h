#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace DearPy3D {

	struct mvTexture
	{
		VkImage        textureImage       = VK_NULL_HANDLE;
		VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
		VkImageView    textureImageView   = VK_NULL_HANDLE;
	};

	mvTexture mvCreateTexture(const std::string& file);
	void      mvCleanupTexture(mvTexture& texture);

}