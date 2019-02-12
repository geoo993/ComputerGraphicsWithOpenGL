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
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} vs_out;

uniform bool bUseScreenQuad;

// This is the entry point into the vertex shader
void main()
{
    
    vec4 position = vec4(inPosition, 1.0f);
    
    
    // Get the vertex normal and vertex position in eye coordinates
    //mat3 normalMatrix = mat3(transpose(inverse(matrices.modelMatrix)));
    vs_out.vWorldNormal = matrices.normalMatrix * inNormal;
    vs_out.vLocalNormal = inNormal;
    
    vs_out.vEyePosition = matrices.projMatrix * matrices.viewMatrix * position;
    vs_out.vWorldPosition = vec3(matrices.modelMatrix * position);
    vs_out.vLocalPosition = inPosition;
    
    if (bUseScreenQuad == true){
        // Pass through the texture coordinate
        vs_out.vTexCoord = inCoord;
        
        gl_Position = vec4(inPosition, 1.0f);
    } else {
        
        // Pass through the texture coordinate
        vs_out.vTexCoord = inCoord;
        
        // Transform the vertex spatial position using
        gl_Position = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix * position;
    }
    
}
