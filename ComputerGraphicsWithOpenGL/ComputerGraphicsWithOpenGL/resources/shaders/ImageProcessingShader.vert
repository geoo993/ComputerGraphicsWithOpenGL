#version 400 core

// Structure for matrices
uniform struct Matrices
{
    mat4 projMatrix;
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 modelViewMatrix;
    mat3 normalMatrix;
    
} matrices;

uniform bool useScreenQuad;
uniform float FXAA_SUBPIX_SHIFT = 1.0f/4.0f;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec2 vTexCoord;	// Texture coordinate
out vec3 vWorldPosition;
out vec3 vLocalNormal;
out vec3 posPos;


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
    
        vWorldPosition = inPosition; 
        
        vLocalNormal = inNormal;
        
    }else {
        
        gl_Position = vec4(inPosition.x, -inPosition.z, -1.0f, 1.0f);
        
        // Pass through the texture coordinate
        vTexCoord = vec2(inCoord.x, 1-inCoord.y);
        
        vWorldPosition = vec3(inPosition.x, -inPosition.z, -1.0f); 
        
        vLocalNormal = inNormal;
        
    }
    
} 

