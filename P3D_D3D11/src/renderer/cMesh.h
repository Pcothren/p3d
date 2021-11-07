#pragma once

#include <string>
#include "cMath.h"
#include "cBuffer.h"
#include "cTexture.h"
#include "cSampler.h"

namespace p3d {

    struct cMesh
    {
        cBuffer       indexBuffer;
        cBuffer       vertexBuffer;
        glm::vec3     pos;
        glm::vec3     rot;
        cTexture      texture;
        cSampler      sampler = cCreateSampler();
    };

    cMesh cCreateTexturedCube(cTexture texture, float size = 1.0f);

}
