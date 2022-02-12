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
		// get globals
		mvPlotContext& gp = *GContext;

		//setting up imgui child and moving to screen drawing coords sort of like the viewport
		ImGui::BeginChild(title_id, size);
		ImVec2 screenSpace = ImGui::GetItemRectSize(); //maybe watch this if the item changes to decide to zoom window?
		gp.drawList = ImGui::GetWindowDrawList();
		ImVec2 pCur = ImGui::GetCursorScreenPos();

		float screen_scale[4]{ 1, 1, 1, 1 };
		float screen_angle[4]{ 0, 0, 0, 1 };
		float screen_translation[4]{ pCur.x, pCur.y + 100, 0, 1 };

		//TODO: Only need to reset and update when plot changes maybe cash with plot system?
		// reset state of transform to identity until cashing is set up
		for (int i = 0; i < 16; ++i)
		{
			gp.mvScreenTransform[i] = p3d_internal::identityMatrix16[i];
		};

		p3d_internal::createCompositionMatrix(&gp.mvScreenTransform[0], &screen_scale[0], &screen_angle[0], &screen_translation[0]);
		return true;
	}

	bool BeginCoordinateSystem()
	{
		mvPlotContext& gp = *GContext;

		// setting up plot transform data sort of like a camera to view up as positive y
		static float coord_scale[4]{ 1, 1, 1, 1 };
		static float coord_angle[4]{ 180, 0, 0, 1 };
		static float coord_translation[4]{ 0, 0, 0, 1 };

		//TODO: Only need to reset and update when coord system changes maybe cash with coord system?
		// reset state of transform to identity until cashing is set up
		for (int i = 0; i < 16; ++i)
		{
			gp.mvCoordinateTransform[i] = p3d_internal::identityMatrix16[i];
		};

		p3d_internal::createCompositionMatrix(&gp.mvCoordinateTransform[0], &coord_scale[0], &coord_angle[0], &coord_translation[0]);
		return true;
	}

	bool AddSeries()
	{
		mvPlotContext& gp = *GContext;

		// setting up series transform data
		static float series_scale[4]{ 1, 1, 1, 1 };
		static float series_angle[4]{ 0, 0, 0, 1 };
		static float series_translation[4]{ 0, 0, 0, 1 };

		//TODO: Only need to reset and update when changing series
		// reset state of transform to identity until cashing is set up
		for (int i = 0; i < 16; ++i)
		{
			gp.mvSeriesTransform[i] = p3d_internal::identityMatrix16[i];
		};

		p3d_internal::createCompositionMatrix(&gp.mvSeriesTransform[0], &series_scale[0], &series_angle[0], &series_translation[0]);
		return true;
	}


	void EndPlot()
	{
		ImGui::EndChild();
	}

	void EndCoordinateSystem() 
	{
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
	void AddLine(const char* label_id, const float* xs, const float* ys, int count, int offset)
	{

		mvPlotContext& gp = *GContext;

		for (int i = 0; i < count-1; ++i)
		{
			// data loaded in as matrix we know its 2 data
			// TODO: load in the ability to do 1x3 matrix or 1x2 depending on input
			float pt1[4]{ xs[i], ys[i], 0.0f, 1.0f };
			float pt2[4]{ xs[i + 1], ys[i + 1], 0.0f, 1.0f };

			//applying transforms to points
			//TODO: maybe consider multiplying the plot and series matrix into one (series probably ownt change plots very often
			p3d_internal::multiply4(&pt1[0], &gp.mvCoordinateTransform[0]);
			p3d_internal::multiply4(&pt2[0], &gp.mvCoordinateTransform[0]);

			p3d_internal::multiply4(&pt1[0], &gp.mvScreenTransform[0]);
			p3d_internal::multiply4(&pt2[0], &gp.mvScreenTransform[0]);

			//drawing line
			const ImU32 col = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255)); 
			gp.drawList->AddLine({ pt1[0], pt1[1] }, { pt2[0], pt2[1] }, col);
		};
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