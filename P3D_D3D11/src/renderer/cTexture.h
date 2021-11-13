#pragma once
#include "cGraphics.h"
#include <string>

namespace p3d {

    struct cTexture
    {
        ID3D11ShaderResourceView* view;
        int index = 0;
        int width = 0;
        int height = 0;
    };

    cTexture cCreateTexture(const std::string& path);

}