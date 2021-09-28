#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inViewPos;
layout(location = 1) in vec3 inViewNormal;
layout(location = 2) in vec3 inWorldNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in mat3 inTangentBasis;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

void main() 
{
    outColor = texture(texSampler, inTexCoord);
    return; 
}