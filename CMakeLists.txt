cmake_minimum_required (VERSION 3.13)

project ("P3D")

add_definitions(-D_USE_MATH_DEFINES)

# Directx 11
add_subdirectory ("P3D_D3D11")
add_subdirectory ("Sandbox_D3D11")
