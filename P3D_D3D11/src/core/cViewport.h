#pragma once

#include <optional>
#include <vector>
#include "cWindows.h"

namespace p3d {


    void               cInitializeViewport(int width, int height);
    std::optional<int> cProcessViewportEvents();

    struct cViewport
    {
        int  width         = 500;
        int  height        = 500;
        HWND hWnd          = nullptr;
        bool resized       = false;    
    };
    
}
