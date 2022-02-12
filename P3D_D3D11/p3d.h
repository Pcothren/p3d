#pragma once

#include <string>

#include "imgui.h"

namespace mvPlot {

    struct mvPlotContext;

    extern mvPlotContext* GContext;

    struct mvPlotIO
    {

    };

    struct mvPlotContext
    {
        mvPlotIO       IO;

        ImDrawList* drawList;

        float mvScreenTransform[16]{ 0.0 };

        float mvCoordinateTransform[16]{ 0.0 };

        float mvSeriesTransform[16]{ 0.0 };
    };

    //-----------------------------------------------------------------------------
    // public API
    //-----------------------------------------------------------------------------
    void mvPlotCreateContext();
    void mvPlotDestroyContext();

    // mvPlot version string.
    #define mvPlot_VERSION "0.1.0"
    // Indicates variable should deduced automatically.
    #define mvPlot_AUTO -1
    // Special color used to indicate that a color should be deduced automatically.
    #define mvPlot_AUTO_COL ImVec4(0,0,0,-1)
    // Macro for templated plotting functions; keeps header clean.
    #define mvPlot_TMP template <typename T> mvPlot_API

    // Forward declarations
    struct mvPlotContext;             // mvPlot context (opaque struct, see mvPlot_internal.h)

    // Enums/Flags
    typedef int ImAxis;               // -> enum ImAxis_
    typedef int mvPlotFlags;          // -> enum mvPlotFlags_
    typedef int mvPlotAxisFlags;      // -> enum mvPlotAxisFlags_
    typedef int mvPlotSubplotFlags;   // -> enum mvPlotSubplotFlags_
    typedef int mvPlotLegendFlags;    // -> enum mvPlotLegendFlags_
    typedef int mvPlotMouseTextFlags; // -> enum mvPlotMouseTextFlags_
    typedef int mvPlotDragToolFlags;  // -> mvPlotDragToolFlags_
    typedef int mvPlotBarGroupsFlags; // -> mvPlotBarGroupsFlags_

    typedef int mvPlotCond;           // -> enum mvPlotCond_
    typedef int mvPlotCol;            // -> enum mvPlotCol_
    typedef int mvPlotStyleVar;       // -> enum mvPlotStyleVar_
    typedef int mvPlotMarker;         // -> enum mvPlotMarker_
    typedef int mvPlotColormap;       // -> enum mvPlotColormap_
    typedef int mvPlotLocation;       // -> enum mvPlotLocation_
    typedef int mvPlotBin;            // -> enum mvPlotBin_

    // Axis indices. The values assigned may change; NEVER hardcode these.
    enum mvPlotAxis_ {
        // horizontal axes
        mvPlotAxis_X1 = 0, // enabled by default
        mvPlotAxis_X2,     // disabled by default
        mvPlotAxis_X3,     // disabled by default
        // vertical axes
        mvPlotAxis_Y1,     // enabled by default
        mvPlotAxis_Y2,     // disabled by default
        mvPlotAxis_Y3,     // disabled by default
        // bookeeping
        mvPlotAxis_COUNT
    };

    // Options for plots (see BeginPlot).
    enum mvPlotFlags_ {
        mvPlotFlags_None = 0,       // default
        mvPlotFlags_NoTitle = 1 << 0,  // the plot title will not be displayed (titles are also hidden if preceeded by double hashes, e.g. "##MyPlot")
        mvPlotFlags_NoLegend = 1 << 1,  // the legend will not be displayed
        mvPlotFlags_NoMouseText = 1 << 2,  // the mouse position, in plot coordinates, will not be displayed inside of the plot
        mvPlotFlags_NoInputs = 1 << 3,  // the user will not be able to interact with the plot
        mvPlotFlags_NoMenus = 1 << 4,  // the user will not be able to open context menus
        mvPlotFlags_NoBoxSelect = 1 << 5,  // the user will not be able to box-select
        mvPlotFlags_NoChild = 1 << 6,  // a child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications)
        mvPlotFlags_NoFrame = 1 << 7,  // the ImGui frame will not be rendered
        mvPlotFlags_Equal = 1 << 8,  // x and y axes pairs will be constrained to have the same units/pixel
        mvPlotFlags_Crosshairs = 1 << 9,  // the default mouse cursor will be replaced with a crosshair when hovered
        mvPlotFlags_AntiAliased = 1 << 10, // plot items will be software anti-aliased (not recommended for high density plots, prefer MSAA)
        mvPlotFlags_CanvasOnly = mvPlotFlags_NoTitle | mvPlotFlags_NoLegend | mvPlotFlags_NoMenus | mvPlotFlags_NoBoxSelect | mvPlotFlags_NoMouseText
    };

    // Options for plot axes (see SetupAxis).
    enum mvPlotAxisFlags_ {
        mvPlotAxisFlags_None = 0,       // default
        mvPlotAxisFlags_NoLabel = 1 << 0,  // the axis label will not be displayed (axis labels also hidden if the supplied string name is NULL)
        mvPlotAxisFlags_NoGridLines = 1 << 1,  // no grid lines will be displayed
        mvPlotAxisFlags_NoTickMarks = 1 << 2,  // no tick marks will be displayed
        mvPlotAxisFlags_NoTickLabels = 1 << 3,  // no text labels will be displayed
        mvPlotAxisFlags_NoInitialFit = 1 << 4,  // axis will not be initially fit to data extents on the first rendered frame
        mvPlotAxisFlags_NoMenus = 1 << 5,  // the user will not be able to open context menus with right-click
        mvPlotAxisFlags_Opposite = 1 << 6,  // axis ticks and labels will be rendered on conventionally opposite side (i.e, right or top)
        mvPlotAxisFlags_Foreground = 1 << 7,  // grid lines will be displayed in the foreground (i.e. on top of data) in stead of the background
        mvPlotAxisFlags_LogScale = 1 << 8,  // a logartithmic (base 10) axis scale will be used (mutually exclusive with mvPlotAxisFlags_Time)
        mvPlotAxisFlags_Time = 1 << 9,  // axis will display date/time formatted labels (mutually exclusive with mvPlotAxisFlags_LogScale)
        mvPlotAxisFlags_Invert = 1 << 10, // the axis will be inverted
        mvPlotAxisFlags_AutoFit = 1 << 11, // axis will be auto-fitting to data extents
        mvPlotAxisFlags_RangeFit = 1 << 12, // axis will only fit points if the point is in the visible range of the **orthogonal** axis
        mvPlotAxisFlags_LockMin = 1 << 13, // the axis minimum value will be locked when panning/zooming
        mvPlotAxisFlags_LockMax = 1 << 14, // the axis maximum value will be locked when panning/zooming
        mvPlotAxisFlags_Lock = mvPlotAxisFlags_LockMin | mvPlotAxisFlags_LockMax,
        mvPlotAxisFlags_NoDecorations = mvPlotAxisFlags_NoLabel | mvPlotAxisFlags_NoGridLines | mvPlotAxisFlags_NoTickMarks | mvPlotAxisFlags_NoTickLabels,
        mvPlotAxisFlags_AuxDefault = mvPlotAxisFlags_NoGridLines | mvPlotAxisFlags_Opposite
    };

    // Options for subplots (see BeginSubplot).
    enum mvPlotSubplotFlags_ {
        mvPlotSubplotFlags_None = 0,       // default
        mvPlotSubplotFlags_NoTitle = 1 << 0,  // the subplot title will not be displayed (titles are also hidden if preceeded by double hashes, e.g. "##MySubplot")
        mvPlotSubplotFlags_NoLegend = 1 << 1,  // the legend will not be displayed (only applicable if mvPlotSubplotFlags_ShareItems is enabled)
        mvPlotSubplotFlags_NoMenus = 1 << 2,  // the user will not be able to open context menus with right-click
        mvPlotSubplotFlags_NoResize = 1 << 3,  // resize splitters between subplot cells will be not be provided
        mvPlotSubplotFlags_NoAlign = 1 << 4,  // subplot edges will not be aligned vertically or horizontally
        mvPlotSubplotFlags_ShareItems = 1 << 5,  // items across all subplots will be shared and rendered into a single legend entry
        mvPlotSubplotFlags_LinkRows = 1 << 6,  // link the y-axis limits of all plots in each row (does not apply to auxiliary axes)
        mvPlotSubplotFlags_LinkCols = 1 << 7,  // link the x-axis limits of all plots in each column (does not apply to auxiliary axes)
        mvPlotSubplotFlags_LinkAllX = 1 << 8,  // link the x-axis limits in every plot in the subplot (does not apply to auxiliary axes)
        mvPlotSubplotFlags_LinkAllY = 1 << 9,  // link the y-axis limits in every plot in the subplot (does not apply to auxiliary axes)
        mvPlotSubplotFlags_ColMajor = 1 << 10  // subplots are added in column major order instead of the default row major order
    };

    // Options for legends (see SetupLegend)
    enum mvPlotLegendFlags_ {
        mvPlotLegendFlags_None = 0,      // default
        mvPlotLegendFlags_NoButtons = 1 << 0, // legend icons will not function as hide/show buttons
        mvPlotLegendFlags_NoHighlightItem = 1 << 1, // plot items will not be highlighted when their legend entry is hovered
        mvPlotLegendFlags_NoHighlightAxis = 1 << 2, // axes will not be highlighted when legend entries are hovered (only relevant if x/y-axis count > 1)
        mvPlotLegendFlags_NoMenus = 1 << 3, // the user will not be able to open context menus with right-click
        mvPlotLegendFlags_Outside = 1 << 4, // legend will be rendered outside of the plot area
        mvPlotLegendFlags_Horizontal = 1 << 5, // legend entries will be displayed horizontally
    };

    // Options for mouse hover text (see SetupMouseText)
    enum mvPlotMouseTextFlags_ {
        mvPlotMouseTextFlags_None = 0,      // default
        mvPlotMouseTextFlags_NoAuxAxes = 1 << 0, // only show the mouse position for primary axes
        mvPlotMouseTextFlags_NoFormat = 1 << 1, // axes label formatters won't be used to render text
        mvPlotMouseTextFlags_ShowAlways = 1 << 2, // always display mouse position even if plot not hovered
    };

    // Options for DragPoint, DragLine, DragRect
    enum mvPlotDragToolFlags_ {
        mvPlotDragToolFlags_None = 0,      // default
        mvPlotDragToolFlags_NoCursors = 1 << 0, // drag tools won't change cursor icons when hovered or held
        mvPlotDragToolFlags_NoFit = 1 << 1, // the drag tool won't be considered for plot fits
        mvPlotDragToolFlags_NoInputs = 1 << 2, // lock the tool from user inputs
        mvPlotDragToolFlags_Delayed = 1 << 3, // tool rendering will be delayed one frame; useful when applying position-constraints
    };

    // Flags for mvPlot::PlotBarGroups
    enum mvPlotBarGroupsFlags_ {
        mvPlotBarGroupsFlags_None = 0,      // default
        mvPlotBarGroupsFlags_Stacked = 1 << 0, // items in a group will be stacked on top of each other
    };

    // Represents a condition for SetupAxisLimits etc. (same as ImGuiCond, but we only support a subset of those enums)
    enum mvPlotCond_
    {
        mvPlotCond_None = ImGuiCond_None,    // No condition (always set the variable), same as _Always
        mvPlotCond_Always = ImGuiCond_Always,  // No condition (always set the variable)
        mvPlotCond_Once = ImGuiCond_Once,    // Set the variable once per runtime session (only the first call will succeed)
    };

    // Plot styling colors.
    enum mvPlotCol_ {
        // item styling colors
        mvPlotCol_Line,          // plot line/outline color (defaults to next unused color in current colormap)
        mvPlotCol_Fill,          // plot fill color for bars (defaults to the current line color)
        mvPlotCol_MarkerOutline, // marker outline color (defaults to the current line color)
        mvPlotCol_MarkerFill,    // marker fill color (defaults to the current line color)
        mvPlotCol_ErrorBar,      // error bar color (defaults to ImGuiCol_Text)
        // plot styling colors
        mvPlotCol_FrameBg,       // plot frame background color (defaults to ImGuiCol_FrameBg)
        mvPlotCol_PlotBg,        // plot area background color (defaults to ImGuiCol_WindowBg)
        mvPlotCol_PlotBorder,    // plot area border color (defaults to ImGuiCol_Border)
        mvPlotCol_LegendBg,      // legend background color (defaults to ImGuiCol_PopupBg)
        mvPlotCol_LegendBorder,  // legend border color (defaults to mvPlotCol_PlotBorder)
        mvPlotCol_LegendText,    // legend text color (defaults to mvPlotCol_InlayText)
        mvPlotCol_TitleText,     // plot title text color (defaults to ImGuiCol_Text)
        mvPlotCol_InlayText,     // color of text appearing inside of plots (defaults to ImGuiCol_Text)
        mvPlotCol_AxisText,      // axis label and tick lables color (defaults to ImGuiCol_Text)
        mvPlotCol_AxisGrid,      // axis grid color (defaults to 25% mvPlotCol_AxisText)
        mvPlotCol_AxisTick,      // axis tick color (defaults to AxisGrid)
        mvPlotCol_AxisBg,        // background color of axis hover region (defaults to transparent)
        mvPlotCol_AxisBgHovered, // axis hover color (defaults to ImGuiCol_ButtonHovered)
        mvPlotCol_AxisBgActive,  // axis active color (defaults to ImGuiCol_ButtonActive)
        mvPlotCol_Selection,     // box-selection color (defaults to yellow)
        mvPlotCol_Crosshairs,    // crosshairs color (defaults to mvPlotCol_PlotBorder)
        mvPlotCol_COUNT
    };

    // Plot styling variables.
    enum mvPlotStyleVar_ {
        // item styling variables
        mvPlotStyleVar_LineWeight,         // float,  plot item line weight in pixels
        mvPlotStyleVar_Marker,             // int,    marker specification
        mvPlotStyleVar_MarkerSize,         // float,  marker size in pixels (roughly the marker's "radius")
        mvPlotStyleVar_MarkerWeight,       // float,  plot outline weight of markers in pixels
        mvPlotStyleVar_FillAlpha,          // float,  alpha modifier applied to all plot item fills
        mvPlotStyleVar_ErrorBarSize,       // float,  error bar whisker width in pixels
        mvPlotStyleVar_ErrorBarWeight,     // float,  error bar whisker weight in pixels
        mvPlotStyleVar_DigitalBitHeight,   // float,  digital channels bit height (at 1) in pixels
        mvPlotStyleVar_DigitalBitGap,      // float,  digital channels bit padding gap in pixels
        // plot styling variables
        mvPlotStyleVar_PlotBorderSize,     // float,  thickness of border around plot area
        mvPlotStyleVar_MinorAlpha,         // float,  alpha multiplier applied to minor axis grid lines
        mvPlotStyleVar_MajorTickLen,       // ImVec2, major tick lengths for X and Y axes
        mvPlotStyleVar_MinorTickLen,       // ImVec2, minor tick lengths for X and Y axes
        mvPlotStyleVar_MajorTickSize,      // ImVec2, line thickness of major ticks
        mvPlotStyleVar_MinorTickSize,      // ImVec2, line thickness of minor ticks
        mvPlotStyleVar_MajorGridSize,      // ImVec2, line thickness of major grid lines
        mvPlotStyleVar_MinorGridSize,      // ImVec2, line thickness of minor grid lines
        mvPlotStyleVar_PlotPadding,        // ImVec2, padding between widget frame and plot area, labels, or outside legends (i.e. main padding)
        mvPlotStyleVar_LabelPadding,       // ImVec2, padding between axes labels, tick labels, and plot edge
        mvPlotStyleVar_LegendPadding,      // ImVec2, legend padding from plot edges
        mvPlotStyleVar_LegendInnerPadding, // ImVec2, legend inner padding from legend edges
        mvPlotStyleVar_LegendSpacing,      // ImVec2, spacing between legend entries
        mvPlotStyleVar_MousePosPadding,    // ImVec2, padding between plot edge and interior info text
        mvPlotStyleVar_AnnotationPadding,  // ImVec2, text padding around annotation labels
        mvPlotStyleVar_FitPadding,         // ImVec2, additional fit padding as a percentage of the fit extents (e.g. ImVec2(0.1f,0.1f) adds 10% to the fit extents of X and Y)
        mvPlotStyleVar_PlotDefaultSize,    // ImVec2, default size used when ImVec2(0,0) is passed to BeginPlot
        mvPlotStyleVar_PlotMinSize,        // ImVec2, minimum size plot frame can be when shrunk
        mvPlotStyleVar_COUNT
    };

    // Marker specifications.
    enum mvPlotMarker_ {
        mvPlotMarker_None = -1, // no marker
        mvPlotMarker_Circle,    // a circle marker
        mvPlotMarker_Square,    // a square maker
        mvPlotMarker_Diamond,   // a diamond marker
        mvPlotMarker_Up,        // an upward-pointing triangle marker
        mvPlotMarker_Down,      // an downward-pointing triangle marker
        mvPlotMarker_Left,      // an leftward-pointing triangle marker
        mvPlotMarker_Right,     // an rightward-pointing triangle marker
        mvPlotMarker_Cross,     // a cross marker (not fillable)
        mvPlotMarker_Plus,      // a plus marker (not fillable)
        mvPlotMarker_Asterisk,  // a asterisk marker (not fillable)
        mvPlotMarker_COUNT
    };

    // Built-in colormaps
    enum mvPlotColormap_ {
        mvPlotColormap_Deep = 0,   // a.k.a. seaborn deep             (qual=true,  n=10) (default)
        mvPlotColormap_Dark = 1,   // a.k.a. matplotlib "Set1"        (qual=true,  n=9 )
        mvPlotColormap_Pastel = 2,   // a.k.a. matplotlib "Pastel1"     (qual=true,  n=9 )
        mvPlotColormap_Paired = 3,   // a.k.a. matplotlib "Paired"      (qual=true,  n=12)
        mvPlotColormap_Viridis = 4,   // a.k.a. matplotlib "viridis"     (qual=false, n=11)
        mvPlotColormap_Plasma = 5,   // a.k.a. matplotlib "plasma"      (qual=false, n=11)
        mvPlotColormap_Hot = 6,   // a.k.a. matplotlib/MATLAB "hot"  (qual=false, n=11)
        mvPlotColormap_Cool = 7,   // a.k.a. matplotlib/MATLAB "cool" (qual=false, n=11)
        mvPlotColormap_Pink = 8,   // a.k.a. matplotlib/MATLAB "pink" (qual=false, n=11)
        mvPlotColormap_Jet = 9,   // a.k.a. MATLAB "jet"             (qual=false, n=11)
        mvPlotColormap_Twilight = 10,  // a.k.a. matplotlib "twilight"    (qual=false, n=11)
        mvPlotColormap_RdBu = 11,  // red/blue, Color Brewer          (qual=false, n=11)
        mvPlotColormap_BrBG = 12,  // brown/blue-green, Color Brewer  (qual=false, n=11)
        mvPlotColormap_PiYG = 13,  // pink/yellow-green, Color Brewer (qual=false, n=11)
        mvPlotColormap_Spectral = 14,  // color spectrum, Color Brewer    (qual=false, n=11)
        mvPlotColormap_Greys = 15,  // white/black                     (qual=false, n=2 )
    };

    // Used to position items on a plot (e.g. legends, labels, etc.)
    enum mvPlotLocation_ {
        mvPlotLocation_Center = 0,                                          // center-center
        mvPlotLocation_North = 1 << 0,                                     // top-center
        mvPlotLocation_South = 1 << 1,                                     // bottom-center
        mvPlotLocation_West = 1 << 2,                                     // center-left
        mvPlotLocation_East = 1 << 3,                                     // center-right
        mvPlotLocation_NorthWest = mvPlotLocation_North | mvPlotLocation_West, // top-left
        mvPlotLocation_NorthEast = mvPlotLocation_North | mvPlotLocation_East, // top-right
        mvPlotLocation_SouthWest = mvPlotLocation_South | mvPlotLocation_West, // bottom-left
        mvPlotLocation_SouthEast = mvPlotLocation_South | mvPlotLocation_East  // bottom-right
    };

    // Enums for different automatic histogram binning methods (k = bin count or w = bin width)
    enum mvPlotBin_ {
        mvPlotBin_Sqrt = -1, // k = sqrt(n)
        mvPlotBin_Sturges = -2, // k = 1 + log2(n)
        mvPlotBin_Rice = -3, // k = 2 * cbrt(n)
        mvPlotBin_Scott = -4, // w = 3.49 * sigma / cbrt(n)
    };

    bool BeginPlot(const char* title_id, const ImVec2& size = ImVec2(-1, 0), mvPlotFlags flags = mvPlotFlags_None);
    void EndPlot();
    bool BeginCoordinateSystem();
    void EndCoordinateSystem();
    bool AddSeries();
    void SetupAxes(const char* x_label, const char* y_label, mvPlotAxisFlags x_flags = mvPlotAxisFlags_None, mvPlotAxisFlags y_flags = mvPlotAxisFlags_None);
    void SetupAxesLimits(double x_min, double x_max, double y_min, double y_max, mvPlotCond cond = mvPlotCond_Once);
    void SetNextMarkerStyle(mvPlotMarker marker = mvPlot_AUTO, float size = mvPlot_AUTO, const ImVec4& fill = mvPlot_AUTO_COL, float weight = mvPlot_AUTO, const ImVec4& outline = mvPlot_AUTO_COL);
    void AddLine(const char* label_id, const float* xs, const float* ys, int count, int offset = 0);
    void PlotText(const char* text, double x, double y, bool vertical = false, const ImVec2& pix_offset = ImVec2(0, 0));
    void PushStyleColor(mvPlotCol idx, const ImVec4& col);
    void PopStyleColor(int count = 1);


}
