#version 400 core

in VS_OUT
{
    vec2 vTexCoordPass;    // Texture coordinate
    vec3 vLocalPositionPass;
    vec3 vLocalNormalPass;
    vec3 vWorldPositionPass;
    vec3 vWorldNormalPass;
    vec3 vWorldTangentPass;
    vec4 vEyePositionPass;
} gs_in[];


out VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec3 vWorldTangent;
    vec4 vEyePosition;
} gs_out;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
    
    for (int i = 0; i < 3; i++) {
        
        gl_Position = gl_in[i].gl_Position;
        
        gs_out.vTexCoord = gs_in[i].vTexCoordPass;
        gs_out.vLocalPosition = gs_in[i].vLocalPositionPass;
        gs_out.vLocalNormal = gs_in[i].vLocalNormalPass;
        gs_out.vWorldPosition = gs_in[i].vWorldPositionPass;
        gs_out.vWorldNormal = gs_in[i].vWorldNormalPass;
        gs_out.vWorldTangent = gs_in[i].vWorldTangentPass;
        gs_out.vEyePosition = gs_in[i].vEyePositionPass;
        
        EmitVertex();
    }
    
    EndPrimitive();
    
}

