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

out vec2 vTexCoord;

void main()
{
	// Transform the point
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 0.0f, 1.0f);

	// Pass through the texture coord
	vTexCoord = inCoord;
}
