#pragma once

#include <string>
#include "cMath.h"
#include "cBuffer.h"

namespace p3d {

    struct cMesh
    {
        cBuffer       indexBuffer;
        cBuffer       vertexBuffer;
        glm::vec3     pos;
        glm::vec3     rot;
    };

    cMesh cCreateTexturedCube(const std::string& path, float size = 1.0f);

}
