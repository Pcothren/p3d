#include "cMesh.h"
#include "p3d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace p3d {

    cMesh cCreateTexturedCube(const std::string& path, float size)
    {

        // ... process data if not NULL ...
        // ... x = width, y = height, n = # 8-bit components per pixel ...
        // ... replace '0' with '1'..'4' to force that many components per pixel
        // ... but 'n' will always be the number that it would have been if you said 0
        int x,y,n;
        unsigned char *data = stbi_load(path.data(), &x, &y, &n, 0);

        //catch fail for now
        assert(data);

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        desc.Width = x;
        desc.Height = y;
        desc.MipLevels = 0;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = data;
        initData.SysMemPitch = static_cast<UINT>(x*n);

        ID3D11Texture2D* tex = nullptr;
        HRESULT hr = GContext->graphics.device->CreateTexture2D(&desc, &initData, &tex);
        assert(SUCCEEDED(hr));
        stbi_image_free(data);

        const float side = size;
        auto vertices = std::vector<float>{
            -side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 0 near side
             side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 1
            -side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 2
             side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 3
            -side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 4 far side
             side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 5
            -side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 6
             side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 7
            -side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 8 left side
            -side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 9
            -side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 10
            -side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 11
             side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 12 right side
             side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 13
             side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 14
             side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 15
            -side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 16 bottom side
             side, -side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 17
            -side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 18
             side, -side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 19
            -side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 20 top side
             side,  side, -side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 21
            -side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 22
             side,  side,  side, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // 23
        };

        static auto indices = std::vector<uint32_t>{
             0,  2,  1,  2,  3,  1,
             4,  5,  7,  4,  7,  6,
             8, 10,  9, 10, 11,  9,
            12, 13, 15, 12, 15, 14,
            16, 17, 18, 18, 17, 19,
            20, 23, 21, 20, 22, 23
        };

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            auto v0 = vertices[14 * indices[i]];
            auto v1 = vertices[14 * indices[i + 1]];
            auto v2 = vertices[14 * indices[i + 2]];

            const auto p0 = glm::vec3{ v0, vertices[14 * indices[i] + 1], vertices[14 * indices[i] + 2] };
            const auto p1 = glm::vec3{ v1, vertices[14 * indices[i + 1] + 1], vertices[14 * indices[i + 1] + 2] };
            const auto p2 = glm::vec3{ v2, vertices[14 * indices[i + 2] + 1], vertices[14 * indices[i + 2] + 2] };

            const auto n = glm::normalize(glm::cross(p1 - p0, p2 - p0));

            vertices[14 * indices[i] + 3] = n[0];
            vertices[14 * indices[i] + 4] = n[1];
            vertices[14 * indices[i] + 5] = n[2];

            vertices[14 * indices[i + 1] + 3] = n[0];
            vertices[14 * indices[i + 1] + 4] = n[1];
            vertices[14 * indices[i + 1] + 5] = n[2];

            vertices[14 * indices[i + 2] + 3] = n[0];
            vertices[14 * indices[i + 2] + 4] = n[1];
            vertices[14 * indices[i + 2] + 5] = n[2];
        }

        cMesh mesh{};

        int vertexCount = vertices.size() / 14;
        int vertexStride = 14 * sizeof(float);
        mesh.vertexBuffer = cCreateBuffer(vertices.data(), vertexCount, vertexStride, D3D11_BIND_VERTEX_BUFFER);
        mesh.indexBuffer = cCreateBuffer(indices.data(), indices.size(), sizeof(uint32_t), D3D11_BIND_INDEX_BUFFER);

        return mesh;
    }

}
