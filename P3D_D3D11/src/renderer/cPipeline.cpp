#include "cPipeline.h"
#include <assert.h>
#include <d3dcompiler.h>
#include "cUtils.h"

namespace p3d {

    struct cPixelShader
    {
        cComPtr<ID3D11PixelShader>  shader;
        cComPtr<ID3DBlob>           blob;
        std::string                 path;
    };

    struct cVertexShader
    {
        cComPtr<ID3D11VertexShader> shader;
        cComPtr<ID3D11InputLayout>  inputLayout;
        cComPtr<ID3DBlob>           blob;
        std::string                 path;
    };

    static cPixelShader cCreatePixelShader(const std::string& path)
    {
        cPixelShader shader{};
        shader.path = path;

        cComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(ToWide(path).c_str(),
            nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0,
            shader.blob.GetAddressOf(), shaderCompileErrorsBlob.GetAddressOf());

        if (FAILED(hResult))
        {
            const char* errorString = NULL;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (shaderCompileErrorsBlob)
                errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();

            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
        }

        hResult = GContext->graphics.device->CreatePixelShader(shader.blob->GetBufferPointer(), shader.blob->GetBufferSize(), nullptr, shader.shader.GetAddressOf());
        assert(SUCCEEDED(hResult));
        return shader;
    }

    static cVertexShader cCreateVertexShader(const std::string& path)
    {
        cVertexShader shader{};
        shader.path = path;

        cComPtr<ID3DBlob> shaderCompileErrorsBlob;
        HRESULT hResult = D3DCompileFromFile(ToWide(path).c_str(),
            nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
            0, 0, shader.blob.GetAddressOf(), shaderCompileErrorsBlob.GetAddressOf());

        if (FAILED(hResult))
        {
            const char* errorString = NULL;
            if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                errorString = "Could not compile shader; file not found";
            else if (shaderCompileErrorsBlob)
                errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();

            MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
        }

        hResult = GContext->graphics.device->CreateVertexShader(shader.blob->GetBufferPointer(),
            shader.blob->GetBufferSize(), nullptr, shader.shader.GetAddressOf());

        assert(SUCCEEDED(hResult));

        std::vector<D3D11_INPUT_ELEMENT_DESC> d3dLayout;
        d3dLayout.push_back(D3D11_INPUT_ELEMENT_DESC{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
        d3dLayout.push_back(D3D11_INPUT_ELEMENT_DESC{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
        d3dLayout.push_back(D3D11_INPUT_ELEMENT_DESC{"Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
        d3dLayout.push_back(D3D11_INPUT_ELEMENT_DESC{"Bitangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
        d3dLayout.push_back(D3D11_INPUT_ELEMENT_DESC{"Texcoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});

        hResult = GContext->graphics.device->CreateInputLayout(d3dLayout.data(),
            (uint32_t)d3dLayout.size(),
            shader.blob->GetBufferPointer(),
            shader.blob->GetBufferSize(),
            shader.inputLayout.GetAddressOf());

        assert(SUCCEEDED(hResult));

        return shader;
    }

    cPipeline cCreatePipeline()
    {
        cPipeline pipeline{};

        pipeline.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
        rasterDesc.CullMode = D3D11_CULL_BACK;
        GContext->graphics.device->CreateRasterizerState(&rasterDesc, pipeline.rasterizationState.GetAddressOf());

		D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
        // Depth test parameters
        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

        // Stencil test parameters
        dsDesc.StencilEnable = true;
        dsDesc.StencilReadMask = 0xFF;
        dsDesc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing
        dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing
        dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        GContext->graphics.device->CreateDepthStencilState(&dsDesc, pipeline.depthStencilState.GetAddressOf());

        D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
        auto& brt = blendDesc.RenderTarget[0];
        brt.BlendEnable = TRUE;
        brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        GContext->graphics.device->CreateBlendState(&blendDesc, pipeline.blendState.GetAddressOf());

        cPixelShader pixelShader = cCreatePixelShader("../../P3D_D3D11/shaders/Solid_PS.hlsl");
        cVertexShader vertexShader = cCreateVertexShader("../../P3D_D3D11/shaders/Solid_VS.hlsl");

        pipeline.pixelShader = pixelShader.shader;
        pipeline.pixelBlob = pixelShader.blob;
        pipeline.vertexShader = vertexShader.shader;
        pipeline.vertexBlob = vertexShader.blob;
        pipeline.inputLayout = vertexShader.inputLayout;
        pipeline.viewport.MinDepth = 0.0f;
        pipeline.viewport.MaxDepth = 1.0f;
        pipeline.viewport.TopLeftX = 0.0f;
        pipeline.viewport.TopLeftY = 0.0f;

        return pipeline;
    }
}
