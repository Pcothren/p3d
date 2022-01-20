#include "p3d.h"
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
		ImGui::BeginChild(title_id);
		ImGui::GetWindowDrawList();
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