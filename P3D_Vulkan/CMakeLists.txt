cmake_minimum_required (VERSION 3.13)

add_library(p3d_vulkan STATIC)

set_property(TARGET p3d_vulkan PROPERTY CXX_STANDARD 17)

target_sources(p3d_vulkan

	PRIVATE

		"mvContext.cpp"

		# core
		"core/mvViewport.cpp"
		"core/mvTimer.cpp"

		# renderer
		"renderer/mvRenderer.cpp"
		"renderer/mvMaterial.cpp"
		"renderer/mvDeletionQueue.cpp"
		"renderer/mvAllocator.cpp"
		"renderer/mvGraphics.cpp"
		"renderer/mvCamera.cpp"
		"renderer/mvBuffer.cpp"
		"renderer/mvPipeline.cpp"
		"renderer/mvShader.cpp"
		"renderer/mvTexture.cpp"
		"renderer/mvMesh.cpp"

		# Vulkan Memory Allocator
		"../Dependencies/VulkanMemoryAllocator/vk_mem_alloc.cpp"

		# imgui
		"../Dependencies/imgui/misc/cpp/imgui_stdlib.cpp"
		"../Dependencies/imgui/imgui.cpp"
		"../Dependencies/imgui/imgui_demo.cpp"
		"../Dependencies/imgui/imgui_draw.cpp"
		"../Dependencies/imgui/imgui_widgets.cpp"
		"../Dependencies/imgui/imgui_tables.cpp"
		"../Dependencies/imgui/backends/imgui_impl_glfw.cpp"
		"../Dependencies/imgui/backends/imgui_impl_vulkan.cpp"
)

target_include_directories(p3d_vulkan

	PRIVATE

		"."
		"core/"
		"renderer/"

	    "$ENV{VK_SDK_PATH}/Include/"
		"../Dependencies/glfw/include/"
		"../Dependencies/glm/"
		"../Dependencies/stb/"
		"../Dependencies/VulkanMemoryAllocator/"
		"../Dependencies/imgui/"
		"../Dependencies/imgui/backends/"
)

target_link_directories(p3d_vulkan

	PRIVATE
		
		"$ENV{VK_SDK_PATH}/Lib"

)

target_link_libraries(p3d_vulkan

	PRIVATE
		vulkan-1
		glfw
)
