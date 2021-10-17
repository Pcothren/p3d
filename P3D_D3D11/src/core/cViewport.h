#pragma once

#include <optional>
#include <vector>
#include "cWindows.h"

namespace p3d {


    void               cInitializeViewport(int width, int height);
    std::optional<int> cProcessViewportEvents();

    struct cViewport
    {
        // Meta
        HWND hWnd = nullptr;

        // Style
        int  width         = 500;
        int  height        = 500;

        // State
        bool resized       = false;    
    };
    
}
