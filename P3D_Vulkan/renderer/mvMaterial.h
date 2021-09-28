#pragma once

#include <memory>
#include "mvPipeline.h"
#include "mvDeletionQueue.h"
#include "mvTexture.h"
#include "mvSampler.h"
#include "mvBuffer.h"
#include "mvMath.h"

namespace DearPy3D {

    struct mvMaterialData
    {
        glm::vec4 materialColor = { 0.45f, 0.45f, 0.85f, 1.0f };
        //-------------------------- ( 16 bytes )

        glm::vec3 specularColor = { 0.18f, 0.18f, 0.18f };
        float specularGloss = 8.0f;
        //-------------------------- ( 16 bytes )

        float normalMapWeight = 1.0f;
        int useTextureMap = false;
        int useNormalMap = false;
        int useSpecularMap = false;
        //-------------------------- ( 16 bytes )

        int useGlossAlpha = false;
        int hasAlpha = false;
        int doLighting = true;
        char _pad1[4];
        //-------------------------- ( 16 bytes )
        
        // 
        //-------------------------- ( 4 * 16 = 64 bytes )
    };

    struct mvMaterialBuffer
    {
        std::vector<mvBuffer> buffers;
    };

    struct mvMaterial
    {
        mvPipeline            pipeline;
        mvTexture             texture;
        mvSampler             sampler;
        uint32_t              offsetIndex = 0u;
        mvMaterialBuffer      materialBuffer;
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet*      descriptorSets;

        uint32_t              _index          = 0u;   // uniform offset index
        uint32_t              _lastImageIndex = 100u; // fake value for first run
    };

    mvMaterial mvCreateMaterial (std::vector<mvMaterialData> materialData, const char* vertexShader, const char* pixelShader);
    void       mvCleanupMaterial(mvMaterial& material);

}
