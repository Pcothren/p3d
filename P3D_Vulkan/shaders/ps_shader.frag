#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inViewPos;
layout(location = 1) in vec3 inViewNormal;
layout(location = 2) in vec3 inWorldNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in mat3 inTangentBasis;

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = vec4(0.0, 1.0, 0.0, 1.0);
    return; 
}