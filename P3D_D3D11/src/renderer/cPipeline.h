#pragma once

#include <string>
#include "p3d.h"

namespace p3d {

    struct cPipeline
    {
        cComPtr<ID3D11PixelShader>       pixelShader;
        cComPtr<ID3D11VertexShader>      vertexShader;
        cComPtr<ID3DBlob>                pixelBlob;
        cComPtr<ID3DBlob>                vertexBlob;
        cComPtr<ID3D11InputLayout>       inputLayout;
        cComPtr<ID3D11BlendState>        blendState;
        cComPtr<ID3D11DepthStencilState> depthStencilState;
        cComPtr<ID3D11RasterizerState>   rasterizationState;
        D3D11_VIEWPORT                   viewport;
        D3D11_PRIMITIVE_TOPOLOGY         topology;
    };

    cPipeline cCreatePipeline();
}
