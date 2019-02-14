#version 400 core

// Structure for matrices
uniform struct Matrices
{
    mat4 projMatrix;
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 modelViewMatrix;
    
} matrices;

uniform bool useScreenQuad;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec2 vTexCoord;	// Texture coordinate

// This is the entry point into the vertex shader
void main()
{
    
    if (useScreenQuad == true){
        
        vec4 position = vec4(inPosition, 1.0f);
        
        // Transform the vertex spatial position using
        //gl_Position = matrices.projMatrix * view * model * position;
        gl_Position = matrices.projMatrix * matrices.modelViewMatrix * position;
        
        // Pass through the texture coordinate
        vTexCoord = inCoord;
        
    }else {
        
        gl_Position = vec4(inPosition.x, -inPosition.z, -1.0f, 1.0f);
        
        // Pass through the texture coordinate
        vTexCoord = vec2(inCoord.x, 1-inCoord.y);
        
    }
    
} 
