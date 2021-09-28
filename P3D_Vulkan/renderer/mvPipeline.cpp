#include "mvPipeline.h"
#include <stdexcept>
#include "mvContext.h"

namespace p3d {

	void mvFinalizePipeline(mvPipeline& pipeline, std::vector<VkDescriptorSetLayout> descriptorSetLayouts)
	{
        //---------------------------------------------------------------------
        // input assembler stage
        //---------------------------------------------------------------------
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        std::vector<VkVertexInputAttributeDescription> attributedescriptions;
        {
            // position 3D
            VkVertexInputAttributeDescription description1{};
            description1.binding = 0;
            description1.location = 0;
            description1.format = VK_FORMAT_R32G32B32_SFLOAT;
            description1.offset = 0;

            // normal
            VkVertexInputAttributeDescription description2{};
            description2.binding = 0;
            description2.location = 1;
            description2.format = VK_FORMAT_R32G32B32_SFLOAT;
            description2.offset = sizeof(float)*3;

            // tangent
            VkVertexInputAttributeDescription description3{};
            description3.binding = 0;
            description3.location = 2;
            description3.format = VK_FORMAT_R32G32B32_SFLOAT;
            description3.offset = sizeof(float) * 6;

            // bitangent
            VkVertexInputAttributeDescription description4{};
            description4.binding = 0;
            description4.location = 3;
            description4.format = VK_FORMAT_R32G32B32_SFLOAT;
            description4.offset = sizeof(float) * 9;

            // texture 2d
            VkVertexInputAttributeDescription description5{};
            description5.binding = 0;
            description5.location = 4;
            description5.format = VK_FORMAT_R32G32_SFLOAT;
            description5.offset = sizeof(float) * 12;

            attributedescriptions.push_back(description1);
            attributedescriptions.push_back(description2);
            attributedescriptions.push_back(description3);
            attributedescriptions.push_back(description4);
            attributedescriptions.push_back(description5);
        }

        VkVertexInputBindingDescription bindingDescriptions{};
        bindingDescriptions.binding = 0;
        bindingDescriptions.stride = sizeof(float) * 14;
        bindingDescriptions.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(1);
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributedescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescriptions;
        vertexInputInfo.pVertexAttributeDescriptions = attributedescriptions.data();

        //---------------------------------------------------------------------
        // vertex shader stage
        //---------------------------------------------------------------------
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = pipeline.vertexShader.shaderModule;
        vertShaderStageInfo.pName = "main";

        //---------------------------------------------------------------------
        // tesselation stage
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        // geometry shader stage
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        // rasterization stage
        //---------------------------------------------------------------------

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)GContext->graphics.swapChainExtent.width;
        viewport.height = (float)GContext->graphics.swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = GContext->graphics.swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        //---------------------------------------------------------------------
        // fragment shader stage
        //---------------------------------------------------------------------
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = pipeline.fragShader.shaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        //---------------------------------------------------------------------
        // color blending stage
        //---------------------------------------------------------------------
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.5f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        //---------------------------------------------------------------------
        // Create Pipeline layout
        //---------------------------------------------------------------------

        //setup push constants
        VkPushConstantRange push_constant;
        push_constant.offset = 0;
        push_constant.size = 192;
        push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
        pipelineLayoutInfo.pPushConstantRanges = &push_constant;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

        if (vkCreatePipelineLayout(mvGetLogicalDevice(), &pipelineLayoutInfo, nullptr, &pipeline.pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("failed to create pipeline layout!");

        //---------------------------------------------------------------------
        // Create Pipeline
        //---------------------------------------------------------------------

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = pipeline.pipelineLayout;
        pipelineInfo.renderPass = GContext->graphics.renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.pDepthStencilState = &depthStencil;

        if (vkCreateGraphicsPipelines(mvGetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.pipeline) != VK_SUCCESS)
            throw std::runtime_error("failed to create graphics pipeline!");

        // no longer need this
        vkDestroyShaderModule(mvGetLogicalDevice(), pipeline.vertexShader.shaderModule, nullptr);
        vkDestroyShaderModule(mvGetLogicalDevice(), pipeline.fragShader.shaderModule, nullptr);
        pipeline.vertexShader.shaderModule = VK_NULL_HANDLE;
        pipeline.fragShader.shaderModule = VK_NULL_HANDLE;

        GContext->graphics.deletionQueue.pushDeletor([=]() {
            vkDestroyPipeline(mvGetLogicalDevice(), pipeline.pipeline, nullptr);
            vkDestroyPipelineLayout(mvGetLogicalDevice(), pipeline.pipelineLayout, nullptr);
            for(auto descriptorSetLayout : descriptorSetLayouts)
                vkDestroyDescriptorSetLayout(mvGetLogicalDevice(), descriptorSetLayout, nullptr);
            });
	}

}