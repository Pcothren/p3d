#pragma once

#include <memory>
#include "mvPipeline.h"
#include "mvDeletionQueue.h"
#include "mvTexture.h"
#include "mvBuffer.h"
#include "mvMath.h"

namespace p3d {


    struct mvMaterial
    {
        mvPipeline            pipeline;
        mvTexture             texture;
        mvSampler             sampler;
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet*      descriptorSets;
    };

    mvMaterial mvCreateMaterial (const char* vertexShader, const char* pixelShader);
    void       mvCleanupMaterial(mvMaterial& material);

}
