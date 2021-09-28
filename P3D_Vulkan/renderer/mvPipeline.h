#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
#include "mvShader.h"

namespace p3d {

	struct mvPipeline
	{
		mvShader         vertexShader;
		mvShader         fragShader;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline       pipeline = VK_NULL_HANDLE;
	};

	void mvFinalizePipeline(mvPipeline& pipeline, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);

}