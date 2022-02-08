#include "p3d.h"
#include "p3d_internal.h"
#include <assert.h>

namespace mvPlot {

	extern mvPlotContext* GContext = nullptr;

	void mvPlotCreateContext()
	{
		if (GContext)
		{
			assert(false && "Context already created.");
			return;
		}

		GContext = new mvPlotContext();
	}

	void mvPlotDestroyContext()
	{
		if (GContext)
		{
			delete GContext;
			GContext = nullptr;
			return;
		}

		assert(false && "Context already destroyed.");
	}

	bool BeginPlot(const char* title_id, const ImVec2& size, mvPlotFlags flags)
	{
		//setting up imgui child and moving to screen drawing coords sort of like the viewport
		ImGui::BeginChild(title_id, size);
		ImVec2 screenSpace = ImGui::GetItemRectSize(); //maybe watch this if the item changes to decide to zoom window?
		ImDrawList* plot = ImGui::GetWindowDrawList();
		ImVec2 pCur = ImGui::GetCursorScreenPos();

		float screen_scale[4]{ 1, 1, 1, 1 };
		float screen_angle[4]{ 0, 0, 0, 1 };
		float screen_translation[4]{ pCur.x, pCur.y + 100, 0, 1 };
		float screenTM[16]{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		p3d_internal::createCompositionMatrix(&screenTM[0], &screen_scale[0], &screen_angle[0], &screen_translation[0]);
		return true;
	}

	bool BeginCoordinateSystem()
	{
		// setting up plot transform data sort of like a camera to view up as positive y
		static float coord_scale[4]{ 1, 1, 1, 1 };
		static float coord_angle[4]{ 180, 0, 0, 1 };
		static float coord_translation[4]{ 0, 0, 0, 1 };
		float coordTM[16]{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		return true;
	}


	void EndPlot()
	{
		ImGui::EndChild();
	}
	void SetupAxes(const char* x_label, const char* y_label, mvPlotAxisFlags x_flags, mvPlotAxisFlags y_flags)
	{
	}
	void SetupAxesLimits(double x_min, double x_max, double y_min, double y_max, mvPlotCond cond)
	{
	}
	void SetNextMarkerStyle(mvPlotMarker marker, float size, const ImVec4& fill, float weight, const ImVec4& outline)
	{
	}
	void PlotLine(const char* label_id, const float* xs, const float* ys, int count, int offset)
	{
	}
	void PlotText(const char* text, double x, double y, bool vertical, const ImVec2& pix_offset)
	{
	}
	void PushStyleColor(mvPlotCol idx, const ImVec4& col)
	{
	}
	void PopStyleColor(int count)
	{
	}
}