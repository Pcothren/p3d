#pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include "mvShader.h"
#include "mvVertexLayout.h"

namespace DearPy3D {

	struct mvPipeline
	{
		mvShader                           vertexShader;
		mvShader                           fragShader;
		mvVertexLayout                     layout;
		VkPipelineLayout                   pipelineLayout = VK_NULL_HANDLE;
		VkPipeline                         pipeline = VK_NULL_HANDLE;
	};

	void mvFinalizePipeline(mvPipeline& pipeline, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);

}