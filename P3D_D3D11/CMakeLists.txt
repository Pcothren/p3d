cmake_minimum_required (VERSION 3.13)

add_library(p3d_d3d11)
set_property(TARGET p3d_d3d11 PROPERTY CXX_STANDARD 17)

target_compile_definitions(p3d_d3d11

	PRIVATE
		$<$<CONFIG:Debug>:MV_DEBUG>
		$<$<CONFIG:Release>:MV_RELEASE>
)

target_sources(p3d_d3d11

	PRIVATE

		"p3d.cpp"
		"p3d_internal.cpp"
		"mvWindowsWindow.cpp"
		"cTimer.cpp"

		# implot
		"../Dependencies/implot/implot.cpp"
		"../Dependencies/implot/implot_items.cpp"
		"../Dependencies/implot/implot_demo.cpp"

		# imgui
		"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"../Dependencies/imgui/imgui_tables.cpp"
		"../Dependencies/imgui/backends/imgui_impl_win32.cpp"
		"../Dependencies/imgui/backends/imgui_impl_dx11.cpp" "p3d_internal.cpp" "p3d_internal.h")

target_include_directories(p3d_d3d11

	PRIVATE

		"."

		"../Dependencies/imgui/"
		"../Dependencies/implot/"
		"../Dependencies/imgui/backends/"

)

target_link_libraries(p3d_d3d11

	PRIVATE
		d3d11
		d3dcompiler
)