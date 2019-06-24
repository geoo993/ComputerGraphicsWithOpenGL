#version 400 core

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
} vs_out;

// This is the entry point into the vertex shader
void main()
{
    // Pass through the texture coordinate
    vs_out.vTexCoord = inCoord;

    gl_Position = vec4(inPosition, 1.0f);
}
