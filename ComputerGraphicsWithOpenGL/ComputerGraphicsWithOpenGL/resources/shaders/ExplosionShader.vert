#version 400 core

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
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;

out VS_OUT
{
    vec2 vTexCoordPass;    // Texture coordinate
    vec3 vLocalPositionPass;
    vec3 vLocalNormalPass;
    vec3 vWorldPositionPass;
    vec3 vWorldNormalPass;
    vec3 vWorldTangentPass;
    vec4 vEyePositionPass;
} vs_out;


// This is the entry point into the vertex shader
void main()
{
    vec4 position = vec4(inPosition, 1.0f);
    vec4 normal = vec4(inNormal, 1.0f);
    
    // Pass through the texture coordinate
    vs_out.vTexCoordPass = inCoord;
    
    // Get the vertex normal and vertex position in eye coordinates
    //mat3 normalMatrix = mat3(transpose(inverse(matrices.modelMatrix)));
    vs_out.vWorldNormalPass = matrices.normalMatrix * inNormal;
    vs_out.vWorldTangentPass = matrices.normalMatrix * inTangent;
    vs_out.vLocalNormalPass = inNormal;
    
    vs_out.vWorldPositionPass = vec3(matrices.modelMatrix * position);
    vs_out.vLocalPositionPass = inPosition;
    vs_out.vEyePositionPass = matrices.projMatrix * matrices.viewMatrix * position;
    
    // Transform the vertex spatial position using
    gl_Position = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix * position;
}
