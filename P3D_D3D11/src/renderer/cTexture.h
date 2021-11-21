#pragma once
#include "cGraphics.h"
#include <string>

namespace p3d {

    struct cTexture
    {
        cComPtr<ID3D11ShaderResourceView> shaderResource;
        cComPtr<ID3D11RenderTargetView> renderTarget;
        int index = 0;
        int width = 0;
        int height = 0;
    };

    cTexture cCreateTexture(const std::string& path);

    cTexture cCreateRenderTargetTexture();

}