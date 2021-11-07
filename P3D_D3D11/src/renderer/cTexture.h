#pragma once
#include "cGraphics.h"
#include <string>

namespace p3d {

    struct cTexture
    {
        ID3D11ShaderResourceView* view;
        int index = 0;
    };

    cTexture cCreateTexture(const std::string& path);

}