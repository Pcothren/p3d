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

using namespace mvPlot;

int main()
{
    auto window = new mvWindowClass();

    mvPlotCreateContext();
    window->show();
    window->setup();
    cTimer timer;
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
                static float xs[2] = { -50,300 };
                static float ys[2] = { 200,300 };
                static ImVec4 translation = { 0, 0, 0, 1};
                static ImVec4 scale = { 0, 0, 0, 1};
                ImGui::DragFloat2("x##mvPlot", &xs[0], 1.0f, -1000.0f, 1000.0f, "%.0f px");
                ImGui::DragFloat2("y##mvPlot", &ys[0], 1.0f, -1000.0f, 1000.0f, "%.0f px");
                ImGui::DragFloat4("translate##mvPlot", &translation.x, 1.0f, -1000.0f, 1000.0f, "%.0f px");
                ImGui::DragFloat4("scale##mvPlot", &scale.x, 1.0f, -1000.0f, 1000.0f, "%.0f px");

                // set up translation matrix
                float tMat[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
                tMat[4 * 0 + 3] = translation.x;
                tMat[4 * 1 + 3] = translation.y;
                tMat[4 * 2 + 3] = translation.z;
                // set up translation matrix
                float sMat[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
                sMat[4 * 0 + 0] = scale.x;
                sMat[4 * 1 + 1] = scale.y;
                sMat[4 * 2 + 2] = scale.z;
                // set up translation matrix
                float rMat[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
                
                ImVec4 pt0 { xs[0], ys[0], 0, 1};
                ImVec4 pt1 { xs[1], ys[1], 0, 1};

                float xp0 = tMat[4 * 0 + 0]*pt0.x + tMat[4 * 0 + 1]*pt0.y +  tMat[4 * 0 + 2] * pt0.z + tMat[4 * 0 + 3] * pt0.w;
                float yp0 = tMat[4 * 1 + 0]*pt0.x + tMat[4 * 1 + 1]*pt0.y +  tMat[4 * 1 + 2] * pt0.z + tMat[4 * 1 + 3] * pt0.w;
                float zp0 = tMat[4 * 2 + 0]*pt0.x + tMat[4 * 2 + 1]*pt0.y +  tMat[4 * 2 + 2] * pt0.z + tMat[4 * 2 + 3] * pt0.w;
                float wp0 = tMat[4 * 3 + 0]*pt0.x + tMat[4 * 3 + 1]*pt0.y +  tMat[4 * 3 + 2] * pt0.z + tMat[4 * 3 + 3] * pt0.w;

                float xp1 = sMat[4 * 0 + 0] * pt1.x + sMat[4 * 0 + 1] * pt1.y + sMat[4 * 0 + 2] * pt1.z + sMat[4 * 0 + 3] * pt1.w;
                float yp1 = sMat[4 * 1 + 0] * pt1.x + sMat[4 * 1 + 1] * pt1.y + sMat[4 * 1 + 2] * pt1.z + sMat[4 * 1 + 3] * pt1.w;
                float zp1 = sMat[4 * 2 + 0] * pt1.x + sMat[4 * 2 + 1] * pt1.y + sMat[4 * 2 + 2] * pt1.z + sMat[4 * 2 + 3] * pt1.w;
                float wp1 = sMat[4 * 3 + 0] * pt1.x + sMat[4 * 3 + 1] * pt1.y + sMat[4 * 3 + 2] * pt1.z + sMat[4 * 3 + 3] * pt1.w;

                ImVec4 pt0Final{ xp0, yp0, zp0, wp0 };
                ImVec4 pt1Final{ xp1, yp1, zp1, wp1 };



                ImGui::BeginChild("##MarkerStylesmvPlot");
                ImDrawList* plot = ImGui::GetWindowDrawList();
                ImVec2 screenSpace = ImGui::GetItemRectSize();
                ImVec2 pCur = ImGui::GetCursorScreenPos();
                ImVec2 p0 = ImVec2(pCur.x + pt0Final.x, pCur.y + pt0Final.y);
                ImVec2 p1 = ImVec2(pCur.x + pt1Final.x, pCur.y + pt1Final.y);
                static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
                const ImU32 col = ImColor(colf);
                const ImU32 col_a = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255));
                plot->AddLine(p0, p1, col, mk_weight);
                ImGui::EndChild();

            }

            ImGui::End();

#if defined (_WIN32)
            ImGui::ShowDemoWindow();
            ImPlot::ShowDemoWindow();
            window->postrender();
#endif
    }
    mvPlotDestroyContext();
}
