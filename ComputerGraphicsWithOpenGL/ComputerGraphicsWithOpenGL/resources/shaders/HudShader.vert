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

uniform bool bUseScreenQuad;

void main()
{
    vec4 position = vec4(inPosition, -1.0f, 1.0f);
    if (bUseScreenQuad) {
        //Set the x,y position on the screen
        gl_Position.xy = position.xy;
        
        //the z position is zero since we are in 2D
        // draw triangles with depth less that 1 so that they do not get culled. -1 makes them visible
        // https://stackoverflow.com/questions/47760298/drawing-multiple-triangles-in-opengl
        gl_Position.z = -1.0f;
        
        //Indicate that the coordinates are normalized
        gl_Position.w = 1.0f;
    } else {
        // Transform the point
        gl_Position = matrices.projMatrix * matrices.modelViewMatrix * position;
    }
    
    // Pass through the texture coord
    vs_out.vTexCoord = inCoord;
}

