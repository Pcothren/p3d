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

        // Default window struct
        cViewport viewport;

        // Creates all comptrs for direct x
        cGraphics graphics;
    };


}
