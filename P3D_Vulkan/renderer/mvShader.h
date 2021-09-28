#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace p3d {

	struct mvShader
	{
		std::string    file;
		VkShaderModule shaderModule = VK_NULL_HANDLE;
	};

	mvShader mvCreateShader(const std::string& file);

}