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

        ID3D11Texture2D* tex = nullptr;
        HRESULT hr = GContext->graphics.device->CreateTexture2D(&desc, &initData, &tex);
        assert(SUCCEEDED(hr));

        stbi_image_free(data);

        // create resource view
        ID3D11ShaderResourceView* resource_view{};
        hr = GContext->graphics.device->CreateShaderResourceView(tex, nullptr, &resource_view);
        assert(SUCCEEDED(hr));

        cTexture texture{};

        texture.view = resource_view;
        texture.width = x;
        texture.height = y;

        return texture;
    }
}