#include <string>
#include <imgui_impl_dx11.h>
#include "p3d.h"
#include "cTimer.h"
#if defined (_WIN32)
#include "mvWindowsWindow.h"
#define mvWindowClass mvWindowsWindow
#endif
#include <iostream>
#include <implot.h>

int main()
{
    auto window = new mvWindowClass();

    mvPlot::mvPlotCreateContext();
    window->show();
    window->setup();
    mvPlot::cTimer timer;
    while (window->m_running)
    {

        const auto dt = timer.mark() * 1.0f;
            window->prerender();

            // Setting up Implot
            ImGuiIO& io = ImGui::GetIO();
            ImGui::GetForegroundDrawList()->AddText(ImVec2(45, 45),
                ImColor(0.0f, 1.0f, 0.0f), std::string(std::to_string(io.Framerate) + " FPS").c_str());

            //mvPlot Testing and performance comparison to imPlot
            ImGui::Begin("mvPlot Testing");

            // Markers and Text
            if (ImGui::CollapsingHeader("Markers and Text")) {
                static float mk_size = ImPlot::GetStyle().MarkerSize;
                static float mk_weight = ImPlot::GetStyle().MarkerWeight;
                ImGui::DragFloat("Marker Size", &mk_size, 0.1f, 2.0f, 10.0f, "%.2f px");
                ImGui::DragFloat("Marker Weight", &mk_weight, 0.05f, 0.5f, 3.0f, "%.2f px");

                if (ImPlot::BeginPlot("##MarkerStyles", ImVec2(-1, 0), ImPlotFlags_CanvasOnly)) {
                    ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(0, 10, 0, 12);

                    ImS8 xs[2] = { 1,4 };
                    ImS8 ys[2] = { 10,11 };

                    // filled markers
                    for (int m = 0; m < ImPlotMarker_COUNT; ++m) {
                        ImGui::PushID(m);
                        ImPlot::SetNextMarkerStyle(m, mk_size, IMPLOT_AUTO_COL, mk_weight);
                        ImPlot::PlotLine("##Filled", xs, ys, 2);
                        ImGui::PopID();
                        ys[0]--; ys[1]--;
                    }
                    xs[0] = 6; xs[1] = 9; ys[0] = 10; ys[1] = 11;
                    // open markers
                    for (int m = 0; m < ImPlotMarker_COUNT; ++m) {
                        ImGui::PushID(m);
                        ImPlot::SetNextMarkerStyle(m, mk_size, ImVec4(0, 0, 0, 0), mk_weight);
                        ImPlot::PlotLine("##Open", xs, ys, 2);
                        ImGui::PopID();
                        ys[0]--; ys[1]--;
                    }

                    ImPlot::PlotText("Filled Markers", 2.5f, 6.0f);
                    ImPlot::PlotText("Open Markers", 7.5f, 6.0f);

                    ImPlot::PushStyleColor(ImPlotCol_InlayText, ImVec4(1, 0, 1, 1));
                    ImPlot::PlotText("Vertical Text", 5.0f, 6.0f, true);
                    ImPlot::PopStyleColor();

                    ImPlot::EndPlot();
                }
            }

            if (ImGui::CollapsingHeader("Markers and Text##mvPlot")) {
                static float mk_size = ImPlot::GetStyle().MarkerSize;
                static float mk_weight = ImPlot::GetStyle().MarkerWeight;
                ImGui::DragFloat("Marker Size##mvPlot", &mk_size, 0.1f, 2.0f, 10.0f, "%.2f px");
                ImGui::DragFloat("Marker Weight##mvPlot", &mk_weight, 0.05f, 0.5f, 3.0f, "%.2f px");

                //if (mvPlot::BeginPlot("##MarkerStylesmvPlot", ImVec2(-1, 0), mvPlotFlags_CanvasOnly)) {
                //    mvPlot::SetupAxes(NULL, NULL, mvPlotAxisFlags_NoDecorations, mvPlotAxisFlags_NoDecorations);
                //    mvPlot::SetupAxesLimits(0, 10, 0, 12);

                //    float xs[2] = { 1,4 };
                //    float ys[2] = { 10,11 };

                //    // filled markers
                //    for (int m = 0; m < mvPlotMarker_COUNT; ++m) {
                //        ImGui::PushID(m);
                //        mvPlot::SetNextMarkerStyle(m, mk_size, IMPLOT_AUTO_COL, mk_weight);
                //        mvPlot::PlotLine("##FilledmvPlot", xs, ys, 2);
                //        ImGui::PopID();
                //        ys[0]--; ys[1]--;
                //    }
                //    xs[0] = 6; xs[1] = 9; ys[0] = 10; ys[1] = 11;
                //    // open markers
                //    for (int m = 0; m < mvPlotMarker_COUNT; ++m) {
                //        ImGui::PushID(m);
                //        mvPlot::SetNextMarkerStyle(m, mk_size, ImVec4(0, 0, 0, 0), mk_weight);
                //        mvPlot::PlotLine("##OpenmvPlot", xs, ys, 2);
                //        ImGui::PopID();
                //        ys[0]--; ys[1]--;
                //    }

                //    mvPlot::PlotText("Filled Markers##mvPlot", 2.5f, 6.0f);
                //    mvPlot::PlotText("Open Markers##mvPlot", 7.5f, 6.0f);

                //    mvPlot::PushStyleColor(mvPlotCol_InlayText, ImVec4(1, 0, 1, 1));
                //    mvPlot::PlotText("Vertical Text##mvPlot", 5.0f, 6.0f, true);
                //    mvPlot::PopStyleColor();

                //    mvPlot::EndPlot();
                //}


                // prearing data
                static float xs[2]{ 0,0 };
                static float ys[2]{ 0,25 };
                ImGui::DragFloat2("x-values##mvPlot", &xs[0], 1.0f, -500.0f, 500.0f, "%.0f px");
                ImGui::DragFloat2("y-values##mvPlot", &ys[0], 1.0f, -500.0f, 500.0f, "%.0f px");


                mvPlot::BeginPlot("##MarkerStylesmvPlot", { 100, 100 });
                mvPlot::BeginCoordinateSystem();
                mvPlot::AddLine("##Line1", xs, ys, 2);
                mvPlot::EndCoordinateSystem();
                mvPlot::EndPlot();

            }

            ImGui::End();

#if defined (_WIN32)
            ImGui::ShowDemoWindow();
            ImPlot::ShowDemoWindow();
            window->postrender();
#endif
    }
    mvPlot::mvPlotDestroyContext();
}
