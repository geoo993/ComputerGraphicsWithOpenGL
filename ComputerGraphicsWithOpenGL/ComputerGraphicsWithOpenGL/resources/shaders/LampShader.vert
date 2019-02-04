#version 410 core

// Structure for matrices
uniform struct Matrices
{
    mat4 projMatrix;
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat3 normalMatrix;
    
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

void main() {
    // Transform the vertex spatial position using
    gl_Position = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix * vec4(inPosition, 1.0f);
}
