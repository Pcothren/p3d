#pragma once

#include <string>

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

    };

    struct cContext
    {
        cIO       IO;
    };


}
