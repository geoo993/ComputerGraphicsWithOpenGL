#version 400 core

// Structure for matrices
uniform struct Matrices
{
    mat4 projMatrix;
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat3 normalMatrix;
    mat4 inverseViewMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec3 vTextureDirection;    // used for skybox

out vec2 vTexCoord;    // Texture coordinate

// This is the entry point into the vertex shader
void main()
{
    vec4 position = vec4(inPosition, 1.0f);
    
    // Save the world position for rendering the skybox
    vTextureDirection = inPosition;
    
    // Pass through the texture coordinate
    vTexCoord = inCoord;
    
    // Transform the vertex spatial position using
    vec4 glPosition = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix * position;
    //vec4 glPosition = projection * matrices.viewMatrixWithoutTranslation * position;
    gl_Position = glPosition.xyww;
    
}
