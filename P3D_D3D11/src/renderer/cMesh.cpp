#include "cMesh.h"
#include "p3d.h"

namespace p3d {

    cMesh cCreateTexturedCube(cTexture texture, float size)
    {
        // creating pixel shader constant buffer to store info
        struct psInfoBuffer {
            glm::vec4 psColor{ 1.0 };
            float texture = true;
            float empty2 = NULL;
            float empty3 = NULL;
            float empty4 = NULL;
        } psInfo;

        D3D11_BUFFER_DESC psInfoDesc;
        psInfoDesc.ByteWidth = sizeof(psInfo);
        psInfoDesc.Usage = D3D11_USAGE_DYNAMIC;
        psInfoDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        psInfoDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        psInfoDesc.MiscFlags = 0;
        psInfoDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = &psInfo;
        InitData.SysMemPitch = 0;
        InitData.SysMemSlicePitch = 0;

        // creating vertex data
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

        HRESULT hr = GContext->graphics.device->CreateBuffer(&psInfoDesc, &InitData, mesh.psInfoBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));

        mesh.texture = texture;

        return mesh;
    }

    cMesh cCreateColorCube(std::vector<float> color, float size)
    {
        // creating pixel shader constant buffer to store info
        struct psInfoBuffer {
            glm::vec4 psColor{ 1.0 };
            float texture = false;
            float empty2 = NULL;
            float empty3 = NULL;
            float empty4 = NULL;
        } psInfo;

        psInfo.psColor = { color[0], color[1], color[2], color[3] };

        D3D11_BUFFER_DESC psInfoDesc;
        psInfoDesc.ByteWidth = sizeof(psInfo);
        psInfoDesc.Usage = D3D11_USAGE_DYNAMIC;
        psInfoDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        psInfoDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        psInfoDesc.MiscFlags = 0;
        psInfoDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = &psInfo;
        InitData.SysMemPitch = 0;
        InitData.SysMemSlicePitch = 0;

        // creating vertex data
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

        HRESULT hr = GContext->graphics.device->CreateBuffer(&psInfoDesc, &InitData, mesh.psInfoBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
        return mesh;
    }

    cMesh cCreateSphere(int LatLines, int LongLines) {

        // creating pixel shader constant buffer to store info
        struct psInfoBuffer {
            glm::vec4 psColor{ 1.0 };
            float texture = false;
            float empty2 = NULL;
            float empty3 = NULL;
            float empty4 = NULL;
        } psInfo;

        int NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
        int NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

        float sphereYaw = 0.0f;
        float spherePitch = 0.0f;

        std::vector<Vertex> vertices(NumSphereVertices);

        XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

        vertices[0].pos.x = 0.0f;
        vertices[0].pos.y = 0.0f;
        vertices[0].pos.z = 1.0f;

        for (DWORD i = 0; i < LatLines - 2; ++i)
        {
            spherePitch = (i + 1) * (3.14 / (LatLines - 1));
            Rotationx = XMMatrixRotationX(spherePitch);
            for (DWORD j = 0; j < LongLines; ++j)
            {
                sphereYaw = j * (6.28 / (LongLines));
                Rotationy = XMMatrixRotationZ(sphereYaw);
                currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
                currVertPos = XMVector3Normalize(currVertPos);
                vertices[i * LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
                vertices[i * LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
                vertices[i * LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
            }
        }

        vertices[NumSphereVertices - 1].pos.x = 0.0f;
        vertices[NumSphereVertices - 1].pos.y = 0.0f;
        vertices[NumSphereVertices - 1].pos.z = -1.0f;


        std::vector<DWORD> indices(NumSphereFaces * 3);

        int k = 0;
        for (DWORD l = 0; l < LongLines - 1; ++l)
        {
            indices[k] = 0;
            indices[k + 1] = l + 1;
            indices[k + 2] = l + 2;
            k += 3;
        }

        indices[k] = 0;
        indices[k + 1] = LongLines;
        indices[k + 2] = 1;
        k += 3;

        for (DWORD i = 0; i < LatLines - 3; ++i)
        {
            for (DWORD j = 0; j < LongLines - 1; ++j)
            {
                indices[k] = i * LongLines + j + 1;
                indices[k + 1] = i * LongLines + j + 2;
                indices[k + 2] = (i + 1) * LongLines + j + 1;

                indices[k + 3] = (i + 1) * LongLines + j + 1;
                indices[k + 4] = i * LongLines + j + 2;
                indices[k + 5] = (i + 1) * LongLines + j + 2;

                k += 6; // next quad
            }

            indices[k] = (i * LongLines) + LongLines;
            indices[k + 1] = (i * LongLines) + 1;
            indices[k + 2] = ((i + 1) * LongLines) + LongLines;

            indices[k + 3] = ((i + 1) * LongLines) + LongLines;
            indices[k + 4] = (i * LongLines) + 1;
            indices[k + 5] = ((i + 1) * LongLines) + 1;

            k += 6;
        }

        for (DWORD l = 0; l < LongLines - 1; ++l)
        {
            indices[k] = NumSphereVertices - 1;
            indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
            indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
            k += 3;
        }

        indices[k] = NumSphereVertices - 1;
        indices[k + 1] = (NumSphereVertices - 1) - LongLines;
        indices[k + 2] = NumSphereVertices - 2;
    }

}
