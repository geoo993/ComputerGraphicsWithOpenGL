#version 400 core

// Structure for matrices
uniform struct Matrices
{
    mat4 projMatrix;
    mat4 modelViewMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inCoord;

out VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} vs_out;

void main()
{
	// Transform the point
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 0.0f, 1.0f);

	// Pass through the texture coord
	vs_out.vTexCoord = inCoord;
}
