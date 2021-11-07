#include <string>
#include <imgui_impl_dx11.h>
#include "p3d.h"
#include "cGraphics.h"
#include "cRenderer.h"
#include "cCamera.h"
#include "cMesh.h"
#include "cTexture.h"
#include "cTimer.h"
#include "cPipeline.h"

using namespace p3d;

int main()
{
    cCreateContext();
    Renderer::cStartRenderer();

    cPipeline pipeline = cCreatePipeline();

    // create camera
    cCamera camera{};
    camera.pos = glm::vec3{ 5.0f, 5.0f, -15.0f };
    camera.aspect = GContext->viewport.width / GContext->viewport.height;

    // create textures
    cTexture brick_texture = cCreateTexture("../../Resources/brickwall.jpg");
    cTexture sun_texture = cCreateTexture("../../Resources/SkyBox/back.png");

    // create meshes
    cMesh cube1 = cCreateTexturedCube(brick_texture, 1.0f);
    cube1.pos.x = 7.0f;
    cube1.pos.y = 7.0f;
    cube1.pos.z = 7.0f;

    cMesh cube2 = cCreateTexturedCube(sun_texture, 1.0f);
    cube2.pos.x = 5.0f;
    cube2.pos.y = 5.0f;
    cube2.pos.z = 5.0f;

    cTimer timer;
    while (true)
    {

        const auto dt = timer.mark() * 1.0f;

        //-----------------------------------------------------------------------------
        // pre draw
        //-----------------------------------------------------------------------------
        if (const auto ecode = cProcessViewportEvents())
            break;

        if (GContext->viewport.resized)
        {
            Renderer::cResize();
            GContext->viewport.resized = false;
            camera.aspect = (float)GContext->viewport.width / (float)GContext->viewport.height;
        }

        // for now, we will just use imgui's input
        if (ImGui::GetIO().KeysDown['W']) cTranslateCamera(camera, 0.0f, 0.0f,   dt);
        if (ImGui::GetIO().KeysDown['S']) cTranslateCamera(camera, 0.0f, 0.0f,  -dt);
        if (ImGui::GetIO().KeysDown['D']) cTranslateCamera(camera,   dt, 0.0f, 0.0f);
        if (ImGui::GetIO().KeysDown['A']) cTranslateCamera(camera,  -dt, 0.0f, 0.0f);
        if (ImGui::GetIO().KeysDown['R']) cTranslateCamera(camera, 0.0f,   dt, 0.0f);
        if (ImGui::GetIO().KeysDown['F']) cTranslateCamera(camera, 0.0f,  -dt, 0.0f);
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::GetIO().WantCaptureMouse) cRotateCamera(camera, ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);

        Renderer::cBeginFrame();

        ImGuiIO& io = ImGui::GetIO();
        ImGui::GetForegroundDrawList()->AddText(ImVec2(45, 45),
            ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());

        //-----------------------------------------------------------------------------
        // user code
        //-----------------------------------------------------------------------------

        ImGui::Begin("Camera Controls");
        ImGui::SliderFloat3("x", &camera.pos.x, -25.0f, 25.0f);
        ImGui::SliderFloat3("y", &camera.pos.y, -25.0f, 25.0f);
        ImGui::SliderFloat3("z", &camera.pos.z, -25.0f, 25.0f);
        ImGui::End();

        ImGui::Begin("Mesh Controls");
        ImGui::SliderFloat3("Cube 1", &cube1.pos.x, -25.0f, 25.0f);
        ImGui::SliderFloat3("Cube 2", &cube2.pos.x, -25.0f, 25.0f);
        ImGui::End();

        glm::mat4 viewMatrix = cBuildCameraMatrix(camera);
        glm::mat4 projMatrix = cBuildProjectionMatrix(camera);

        Renderer::cRenderMesh(cube1, pipeline, viewMatrix, projMatrix);
        Renderer::cRenderMesh(cube2, pipeline, viewMatrix, projMatrix);

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------
        Renderer::cEndFrame();
        Renderer::cPresent();
    }

    Renderer::cStopRenderer();
    cDestroyContext();
}
