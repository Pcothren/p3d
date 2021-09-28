cmake_minimum_required (VERSION 3.13)

function(add_shader SHADER)
    find_program(GLSLC glslc)

    set(current-shader-path "${CMAKE_CURRENT_SOURCE_DIR}/../P3D_Vulkan/shaders/${SHADER}")
    set(current-output-path "${CMAKE_CURRENT_SOURCE_DIR}/../P3D_Vulkan/shaders/${SHADER}.spv")

    # Add a custom command to compile GLSL to SPIR-V.
    get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})

    add_custom_command(
           OUTPUT ${current-output-path}
           COMMAND ${GLSLC} -o ${current-output-path} ${current-shader-path}
           DEPENDS ${current-shader-path}
           IMPLICIT_DEPENDS CXX ${current-shader-path}
           VERBATIM)

    # Make sure our build depends on this output.
    set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
    target_sources(sandbox_vulkan PRIVATE ${current-output-path})
endfunction(add_shader)

add_executable (sandbox_vulkan)

set_property(TARGET sandbox_vulkan PROPERTY CXX_STANDARD 17)

if(MV_NO_SHADERS)
else()
	add_shader(ps_shader.frag)
	add_shader(vs_shader.vert)
endif()

target_sources(sandbox_vulkan

	PRIVATE

		"main.cpp"
)

target_include_directories(sandbox_vulkan

	PRIVATE

		"../P3D_Vulkan/"
		"../P3D_Vulkan/core/"
		"../P3D_Vulkan/renderer/"

	    "$ENV{VK_SDK_PATH}/Include/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
		"../Dependencies/VulkanMemoryAllocator/"
		"../Dependencies/imgui/"
)

target_link_directories(sandbox_vulkan

	PRIVATE
		
		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(sandbox_vulkan

	PRIVATE

		$<$<PLATFORM_ID:Linux>:vulkan>
		$<$<PLATFORM_ID:Windows>:vulkan-1>
		p3d_vulkan
		glfw
)

set_target_properties(sandbox_vulkan
  PROPERTIES
  CXX_STANDARD 17
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}$<$<CONFIG:Release>:/cmake-build-release/>$<$<CONFIG:Debug>:/cmake-build-debug/>Sandbox_Vulkan/"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}$<$<CONFIG:Release>:/cmake-build-release/>$<$<CONFIG:Debug>:/cmake-build-debug/>Sandbox_Vulkan/"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}$<$<CONFIG:Release>:/cmake-build-release/>$<$<CONFIG:Debug>:/cmake-build-debug/>Sandbox_Vulkan/"
  )
