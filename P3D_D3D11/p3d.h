#pragma once

#include <string>
#include "cViewport.h"
#include "cGraphics.h"

namespace p3d {

    struct cContext;

    extern cContext* GContext;

    //-----------------------------------------------------------------------------
    // public API
    //-----------------------------------------------------------------------------
    void cCreateContext();
    void cDestroyContext();

    struct cIO
    {
        std::string        shaderDirectory;
    };

    struct cContext
    {
        cIO       IO;
        cViewport viewport;
        cGraphics graphics;
    };


}
