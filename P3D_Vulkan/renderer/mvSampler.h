#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace DearPy3D {

	struct mvSampler
	{
		VkSampler textureSampler;
	};

	mvSampler mvCreateSampler();
	void      mvCleanupSampler(mvSampler& sampler);

}