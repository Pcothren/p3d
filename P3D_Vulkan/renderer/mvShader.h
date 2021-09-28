#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace DearPy3D {

	struct mvShader
	{
		std::string    file;
		VkShaderModule shaderModule = VK_NULL_HANDLE;
	};

	mvShader mvCreateShader(const std::string& file);

}