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

		# core
		"src/core/cViewport.cpp"
		"src/core/cUtils.cpp"
		"src/core/cTimer.cpp"

		# renderer
		"src/renderer/cGraphics.cpp"
		"src/renderer/cBuffer.cpp"
		"src/renderer/cRenderer.cpp"
		"src/renderer/cCamera.cpp"
		"src/renderer/cMesh.cpp"
		"src/renderer/cPipeline.cpp"

		# imgui
		"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"../Dependencies/imgui/imgui_tables.cpp"
		"../Dependencies/imgui/backends/imgui_impl_win32.cpp"
		"../Dependencies/imgui/backends/imgui_impl_dx11.cpp"
   "src/renderer/cTexture.h" "src/renderer/cTexture.cpp" "src/renderer/cSampler.cpp" "src/renderer/cSampler.h")

target_include_directories(p3d_d3d11

	PRIVATE

		"."
		"src/core/"
		"src/renderer/"
		"shaders/"
		"shaders/common/"

		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
		"../Dependencies/stb/"
		"../Dependencies/glm/"
		"../Dependencies/json/"

)

target_link_libraries(p3d_d3d11

	PRIVATE
		d3d11
		d3dcompiler
)