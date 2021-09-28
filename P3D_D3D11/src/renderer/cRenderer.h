#pragma once

#include "p3d.h"
#include "cMath.h"
#include "cMesh.h"
#include "cPipeline.h"

namespace p3d {

    namespace Renderer
    {
        void cResize();
        void cStartRenderer();
        void cStopRenderer();
        void cBeginFrame();
        void cEndFrame();
        void cPresent();
        void cRenderMesh(cMesh& mesh, cPipeline& pipeline, glm::mat4 camera, glm::mat4 projection);
    }

}
