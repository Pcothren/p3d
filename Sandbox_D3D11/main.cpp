#include <string>
#include <imgui_impl_dx11.h>
#include "p3d.h"
#include "cTimer.h"
#if defined (_WIN32)
#include "mvWindowsWindow.h"
#define mvWindowClass mvWindowsWindow
#elif defined(__APPLE__)
#include "mvAppleWindow.h"
#define mvWindowClass mvAppleWindow
#else
#include "mvLinuxWindow.h"
#define mvWindowClass mvLinuxWindow
#endif
#include <iostream>

using namespace p3d;

int main()
{
    auto window = new mvWindowClass();
    cCreateContext();

    cTimer timer;
    while (true)
    {

        const auto dt = timer.mark() * 1.0f;


        window->show();

        window->setup();
        while (window->m_running)
        {
            window->prerender();

            // Imgui
            ImGuiIO& io = ImGui::GetIO();
            ImGui::GetForegroundDrawList()->AddText(ImVec2(45, 45),
                ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());

            ImGui::Begin("Camera Controls");
            ImGui::End();

            ImGui::Begin("Mesh Controls");
            ImGui::End();

#if defined (_WIN32)
            ImGui::ShowDemoWindow();
            window->postrender();
#elif defined(__APPLE__)
#else
            ImGui::ShowDemoWindow();
            ImPlot::ShowDemoWindow();
            window->postrender();
#endif


        }
    }
    cDestroyContext();
}
