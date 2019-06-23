#version 400 core

// Structure for matrices
uniform struct Matrices
{
    mat4 projMatrix;
    mat4 viewMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition; // used for skybox
} vs_out;

// This is the entry point into the vertex shader
void main()
{
    vec4 position = vec4(inPosition, 1.0f);
    
    // Pass through the texture coordinate
    vs_out.vTexCoord = inCoord;
    
    // Save the world position for rendering the skybox
    vs_out.vLocalPosition = inPosition;
    
    // Transform the vertex spatial position using
    gl_Position = matrices.projMatrix * matrices.viewMatrix * position;
}
