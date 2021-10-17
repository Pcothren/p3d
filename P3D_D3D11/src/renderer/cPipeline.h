#pragma once

#include <string>
#include "p3d.h"

namespace p3d {

    struct cPipeline
    {
        D3D11_PRIMITIVE_TOPOLOGY         topology;
        cComPtr<ID3D11InputLayout>       inputLayout;
        cComPtr<ID3D11VertexShader>      vertexShader;
        cComPtr<ID3DBlob>                vertexBlob;
        cComPtr<ID3D11RasterizerState>   rasterizationState;
        cComPtr<ID3D11PixelShader>       pixelShader;
        cComPtr<ID3DBlob>                pixelBlob;
        cComPtr<ID3D11DepthStencilState> depthStencilState;
        cComPtr<ID3D11BlendState>        blendState;
        D3D11_VIEWPORT                   viewport;
    };

    cPipeline cCreatePipeline();
}
