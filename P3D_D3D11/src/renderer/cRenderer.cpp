#include "cRenderer.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "p3d.h"
#include "cGraphics.h"
#include "cBuffer.h"

namespace p3d {

    namespace Renderer {

        void cResize()
        {
            cRecreateSwapChain();
        }

        void cStartRenderer()
        {
            cInitializeViewport(1850, 900);
            cSetupGraphics("../../P3D_D3D11/shaders/");

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            //io.IniFilename = nullptr;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

            ImGui::StyleColorsDark();

            ImGui_ImplWin32_Init(GContext->viewport.hWnd);
            ImGui_ImplDX11_Init(GContext->graphics.device.Get(), GContext->graphics.imDeviceContext.Get());
        }

        void cStopRenderer()
        {
            // Cleanup
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();

            cCleanupGraphics();
        }

        void cBeginFrame()
        {
            // clear render target
            float backgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            GContext->graphics.imDeviceContext->ClearRenderTargetView(GContext->graphics.target.Get(), backgroundColor);
            GContext->graphics.imDeviceContext->ClearDepthStencilView(GContext->graphics.targetDepth.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            GContext->graphics.imDeviceContext->OMSetRenderTargets(1, GContext->graphics.target.GetAddressOf(), GContext->graphics.targetDepth.Get());

        }

        void cEndFrame()
        {
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }

        void cPresent()
        {
            GContext->graphics.swapChain->Present(1, 0);
        }

        void cRenderMesh(cMesh& mesh, cPipeline& pipeline, glm::mat4 camera, glm::mat4 projection)
        {

            // pipeline
            pipeline.viewport.Width = GContext->viewport.width;
            pipeline.viewport.Height = GContext->viewport.height;
            GContext->graphics.imDeviceContext->RSSetViewports(1u, &pipeline.viewport);
            GContext->graphics.imDeviceContext->IASetPrimitiveTopology(pipeline.topology);
            GContext->graphics.imDeviceContext->RSSetState(pipeline.rasterizationState.Get());
            GContext->graphics.imDeviceContext->OMSetBlendState(pipeline.blendState.Get(), nullptr, 0xFFFFFFFFu);
            GContext->graphics.imDeviceContext->OMSetDepthStencilState(pipeline.depthStencilState.Get(), 0xFF);;
            GContext->graphics.imDeviceContext->IASetInputLayout(pipeline.inputLayout.Get());
            GContext->graphics.imDeviceContext->VSSetShader(pipeline.vertexShader.Get(), nullptr, 0);
            GContext->graphics.imDeviceContext->PSSetShader(pipeline.pixelShader.Get(), nullptr, 0);
            GContext->graphics.imDeviceContext->HSSetShader(nullptr, nullptr, 0);
            GContext->graphics.imDeviceContext->DSSetShader(nullptr, nullptr, 0);
            GContext->graphics.imDeviceContext->GSSetShader(nullptr, nullptr, 0);
            
            cTransforms transforms{};

            transforms.model = glm::translate(mesh.pos) *
                glm::rotate(mesh.rot.x, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
                glm::rotate(mesh.rot.y, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
                glm::rotate(mesh.rot.z, glm::vec3{ 0.0f, 0.0f, 1.0f });
            transforms.modelView = camera * transforms.model;
            transforms.modelViewProjection = projection * camera * transforms.model;

            D3D11_MAPPED_SUBRESOURCE mappedSubresource;
            GContext->graphics.imDeviceContext->Map(GContext->graphics.tranformCBuf.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource);
            memcpy(mappedSubresource.pData, &transforms, sizeof(cTransforms));
            GContext->graphics.imDeviceContext->Unmap(GContext->graphics.tranformCBuf.Get(), 0u);

            // mesh
            const UINT offset = 0u;
            GContext->graphics.imDeviceContext->VSSetConstantBuffers(0u, 1u, GContext->graphics.tranformCBuf.GetAddressOf());
            GContext->graphics.imDeviceContext->IASetIndexBuffer(mesh.indexBuffer.buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
            GContext->graphics.imDeviceContext->IASetVertexBuffers(0u, 1u, mesh.vertexBuffer.buffer.GetAddressOf(), &mesh.vertexBuffer.stride, &offset);
            
            // draw
            GContext->graphics.imDeviceContext->DrawIndexed(mesh.indexBuffer.count, 0u, 0u);
        }
    }

}