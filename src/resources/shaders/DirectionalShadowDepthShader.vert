#version 400 core

// Structure for matrices
uniform struct Matrices
{
    mat4 projMatrix;
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat3 normalMatrix;
    
    // we use a different projection and view matrix to render the scene from the light's point of view.
    mat4 lightSpaceMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;

out VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} vs_out;

// This is the entry point into the vertex shader
void main()
{
    
    vec4 position = vec4(inPosition, 1.0f);

    // Transform the vertex spatial position using
    gl_Position = matrices.lightSpaceMatrix * matrices.modelMatrix * position;
    
    
}
