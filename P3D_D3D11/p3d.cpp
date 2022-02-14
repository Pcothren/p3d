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

		// set screen translation
		gp.screen.translation[0] = pCur.x;
		gp.screen.translation[1] = pCur.y + size.y;

		// set screen rotation to change origin
		gp.screen.angle[0] = 180;

		//TODO: Only need to reset and update when plot changes maybe cash with plot system?
		// reset state of transform to identity until cashing is set up
		for (int i = 0; i < 16; ++i)
		{
			gp.screen.mat[i] = p3d_internal::identityMatrix16[i];
		};

		p3d_internal::modifyCompositionMatrix(&gp.screen.mat[0], &gp.screen.scale[0], &gp.screen.angle[0], &gp.screen.translation[0]);
		return true;
	}

	bool BeginCoordinateSystem(const char* title_id)
	{
		mvPlotContext& gp = *GContext;

		//TODO: Only need to reset and update when coord system changes maybe cash with coord system?
		// reset state of transform to identity until cashing is set up
		for (int i = 0; i < 16; ++i)
		{
			gp.coordinateSystem.mat[i] = p3d_internal::identityMatrix16[i];
		};

		p3d_internal::modifyCompositionMatrix(&gp.coordinateSystem.mat[0], &gp.coordinateSystem.scale[0], &gp.coordinateSystem.angle[0], &gp.coordinateSystem.translation[0]);
		return true;
	}

	void EndPlot()
	{
		ImGui::EndChild();
	}

	void EndCoordinateSystem() 
	{
	}

	void Scale(float x, float y, float z, const char* item)
	{
		mvPlotContext& gp = *GContext;
		static float scale[4]{x,y,z,1.0};
		p3d_internal::modifyCompositionMatrix(&gp.coordinateSystem.mat[0], &scale[0]);
	}

	void Translate(float x, float y, float z, const char* item)
	{
		mvPlotContext& gp = *GContext;
		static float translate[4]{ x,y,z,1.0 };
		p3d_internal::modifyCompositionMatrix(&gp.coordinateSystem.mat[0], nullptr, nullptr ,&translate[0]);
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
	void AddSeriesLine(const char* label_id, const float* xs, const float* ys, const float*zs, int count, int offset)
	{

		mvPlotContext& gp = *GContext;

		//TODO: Only need to reset and update when changing series
		// reset state of transform to identity until cashing is set up
		for (int i = 0; i < 16; ++i)
		{
			gp.series.mat[i] = p3d_internal::identityMatrix16[i];
		};

		//applying any series transform modifiers
		p3d_internal::modifyCompositionMatrix(&gp.series.mat[0], &gp.series.scale[0], &gp.series.angle[0], &gp.series.translation[0]);
		
		// applying transforms to this series
		p3d_internal::multiply16(&gp.series.mat[0], &gp.coordinateSystem.mat[0]);
		p3d_internal::multiply16(&gp.series.mat[0], &gp.screen.mat[0]);

		for (int i = 0; i < count-1; ++i)
		{
			// data loaded in as matrix we know its 2 data
			// TODO: load in the ability to do 1x3 matrix or 1x2 depending on input
			float pt1[4]{ xs[i], ys[i], zs[i], 1.0f};
			float pt2[4]{ xs[i + 1], ys[i + 1], zs[i + 1], 1.0f };

			//applying transforms to points
			//TODO: maybe consider cashing
			p3d_internal::multiply4(&pt1[0], &gp.series.mat[0]);
			p3d_internal::multiply4(&pt2[0], &gp.series.mat[0]);

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