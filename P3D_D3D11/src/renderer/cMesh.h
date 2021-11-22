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
        cComPtr<ID3D11Buffer> psInfoBuffer;
        glm::vec3     pos;
        glm::vec3     rot;
        cTexture      texture;
        cSampler      sampler = cCreateSampler(); // probably move to the texture and keep there
    };

    cMesh cCreateTexturedCube(cTexture texture, float size = 1.0f);
    cMesh cCreateColorCube(std::vector<float> color, float size = 1.0f);
    cMesh cCreateSphere(int LatLines, int LongLines);

}
