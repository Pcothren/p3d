#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "mvViewport.h"
#include "mvGraphics.h"

#define MV_MAX_FRAMES_IN_FLIGHT 2

namespace DearPy3D {

	//-----------------------------------------------------------------------------
    // forward declarations
    //-----------------------------------------------------------------------------
	struct mvContext;
    struct mvIO;


    //-----------------------------------------------------------------------------
    // public API
    //-----------------------------------------------------------------------------
	extern mvContext* GContext;

    struct mvIO
    {
        std::string shaderDirectory;
    };

    struct mvContext
    {
        mvIO       IO;
        mvViewport viewport;
        mvGraphics graphics;
    };

    void CreateContext();
    void DestroyContext();
}