#include <string>
#include <imgui_impl_dx11.h>
#include "p3d.h"
#include "p3d_internal.h"
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
                static float xs[2] { 0,0 };
                static float ys[2] { 0,25 };
                ImGui::DragFloat2("x-values##mvPlot", &xs[0], 1.0f, -500.0f, 500.0f, "%.0f px");
                ImGui::DragFloat2("y-values##mvPlot", &ys[0], 1.0f, -500.0f, 500.0f, "%.0f px");


                mvPlot::BeginPlot("##MarkerStylesmvPlot", { 100, 100 });

                // setting up plot transform data sort of like a camera to view up as positive y
                static float plot_scale[4]{ 1, 1, 1, 1 };
                static float plot_angle[4]{ 180, 0, 0, 1 };
                static float plot_translation[4]{ 0, 0, 0, 1 };
                float plotTM[16]{
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                };

                ////Here to allow for testing
                //ImGui::DragFloat3("plot scale##mvPlot", &plot_scale[0], .1f, -3.0f, 3.0f, "%.0f px");
                //ImGui::DragFloat3("plot angle##mvPlot", &plot_angle[0], 1.0f, -360.0f, 360.0f, "%.0f px");
                //ImGui::DragFloat3("plot translate##mvPlot", &plot_translation[0], 1.0f, -500.0f, 500.0f, "%.0f px");
                p3d_internal::createCompositionMatrix(&plotTM[0], &plot_scale[0], &plot_angle[0], &plot_translation[0]);


                //PLOTTING line
                // data loaded in as series
                float pt1[4]{ xs[0], ys[0], 0.0f, 1.0f };
                float pt2[4]{ xs[1], ys[1], 0.0f, 1.0f };
                // setting up series transform data
                static float series_scale[4]{ 1, 1, 1, 1 };
                static float series_angle[4]{ 0, 0, 0, 1 };
                static float series_translation[4]{ 0, 0, 0, 1 };
                float seriesTM[16]{
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                };
                ////here to allow for testing
                //ImGui::DragFloat3("series scale##mvPlot", &series_scale[0], .1f, -3.0f, 3.0f, "%.0f px");
                //ImGui::DragFloat3("series rotate##mvPlot", &series_angle[0], 1.0f, -360.f, 360.0f, "%.0f px");
                //ImGui::DragFloat3("series translate##mvPlot", &series_translation[0], 1.0f, -500.0f, 500.0f, "%.0f px");
                p3d_internal::createCompositionMatrix(&seriesTM[0], &series_scale[0], &series_angle[0], &series_translation[0]);


                //applying transforms to points
                //TODO: maybe consider multiplying the plot and series matrix into one (series probably ownt change plots very often
                p3d_internal::multiply4(&pt1[0], &seriesTM[0]);
                p3d_internal::multiply4(&pt2[0], &seriesTM[0]);

                p3d_internal::multiply4(&pt1[0], &plotTM[0]);
                p3d_internal::multiply4(&pt2[0], &plotTM[0]);

                p3d_internal::multiply4(&pt1[0], &screenTM[0]);
                p3d_internal::multiply4(&pt2[0], &screenTM[0]);

                //drawing line
                const ImU32 col = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255));
                plot->AddLine({pt1[0], pt1[1]}, { pt2[0], pt2[1] }, col, mk_weight);

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
