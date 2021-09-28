#pragma once

#include <memory>
#include "mvMath.h"
#include "mvBuffer.h"
#include "mvPipeline.h"
#include "mvDeletionQueue.h"
#include "mvMaterial.h"

namespace DearPy3D {

    struct mvMesh
    {
        mvBuffer  indexBuffer;
        mvBuffer  vertexBuffer;
        glm::vec3 pos;
        glm::vec3 rot;
    };

    mvMesh mvCreateTexturedCube(const std::string& path, float sideLength = 1.0f);
    mvMesh mvCreateTexturedQuad(const std::string& path, float sideLength = 1.0f);
    void   mvCleanupMesh(mvMesh& mesh);
        
}
