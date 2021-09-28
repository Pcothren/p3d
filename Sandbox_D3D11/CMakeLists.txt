cmake_minimum_required (VERSION 3.13)

add_executable(sandbox_d3d11)
set_property(TARGET sandbox_d3d11 PROPERTY CXX_STANDARD 17)

target_sources(sandbox_d3d11

	PRIVATE

		"main.cpp"
)

target_include_directories(sandbox_d3d11

	PRIVATE

		"../P3D_D3D11/"
		"../P3D_D3D11/src/core/"
		"../P3D_D3D11/src/renderer/"

		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
		"../Dependencies/stb/"
		"../Dependencies/glm/"

)

target_link_libraries(sandbox_d3d11
	PRIVATE
		p3d_d3d11
)