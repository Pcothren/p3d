#pragma once
#include "cGraphics.h"
#include <string>

namespace p3d {

    struct cSampler
    {
        cComPtr<ID3D11SamplerState> sampler_state;
    };

    cSampler cCreateSampler();

}