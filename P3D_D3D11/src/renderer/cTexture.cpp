#include "cTexture.h"
#include "p3d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace p3d {

    cTexture cCreateTexture(const std::string& path)
    {
        // ... process data if not NULL ...
        // ... x = width, y = height, n = # 8-bit components per pixel ...
        // ... replace '0' with '1'..'4' to force that many components per pixel
        // ... but 'n' will always be the number that it would have been if you said 0
        int x, y, n;
        unsigned char* data = stbi_load(path.data(), &x, &y, &n, 4);

        //catch fail for now
        assert(data);

        // create texture sub resource
        D3D11_SUBRESOURCE_DATA initData{};
        initData.pSysMem = data;
        initData.SysMemPitch = static_cast<UINT>(x * 4);

        // Create texture
        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = x;
        desc.Height = y;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        cComPtr<ID3D11Texture2D> tex;
        HRESULT hr = GContext->graphics.device->CreateTexture2D(&desc, &initData, tex.GetAddressOf());
        assert(SUCCEEDED(hr));

        stbi_image_free(data);

        // create resource view
        cComPtr<ID3D11ShaderResourceView> resource_view;
        hr = GContext->graphics.device->CreateShaderResourceView(tex.Get(), nullptr, resource_view.GetAddressOf());
        assert(SUCCEEDED(hr));

        cTexture texture{};

        texture.shaderResource = resource_view;
        texture.width = x;
        texture.height = y;

        return texture;
    }

    cTexture cCreateRenderTargetTexture()
    {
        // create for texture used in first render pass
        cComPtr<ID3D11Texture2D> textureBuffer;
        cComPtr<ID3D11ShaderResourceView> targetTextureView;
        // create render target
        D3D11_TEXTURE2D_DESC textureDesc;
        textureDesc.Width = GContext->viewport.width;
        textureDesc.Height = GContext->viewport.height;
        textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.ArraySize = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.MiscFlags = 0;
        textureDesc.MipLevels = 1;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        // create 2D texture
        GContext->graphics.device->CreateTexture2D(&textureDesc, NULL, textureBuffer.GetAddressOf());
        // create view of the frame buffer
        GContext->graphics.device->CreateShaderResourceView(textureBuffer.Get(), NULL, targetTextureView.GetAddressOf());
        // create the target view on the texture
        cComPtr<ID3D11RenderTargetView> targetTexture;
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = textureDesc.Format;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
        HRESULT hResult = GContext->graphics.device->CreateRenderTargetView(textureBuffer.Get(), &rtvDesc, targetTexture.GetAddressOf());
        assert(SUCCEEDED(hResult));

        cTexture texture{};
        texture.renderTarget = targetTexture;
        texture.shaderResource = targetTextureView;
        texture.width = GContext->viewport.width;
        texture.height = GContext->viewport.height;

        return texture;
    }
}