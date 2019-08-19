#version 400 core

// https://learnopengl.com/#!Advanced-OpenGL/Geometry-Shader

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

uniform bool bExplode, bAnimate;
uniform float time, magnitude;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}  

vec4 explode(vec4 position, vec3 normal, float t, float m)
{
    vec3 direction = normal * ((sin(t) + 1.0f) / 2.0f) * m; 
    return position + vec4(direction, 0.0f);
} 


void main() {
    
    
    vec3 normal = GetNormal();
    float mag = bExplode ? magnitude : 1.0f;
    float t = bAnimate ? time : 0.0f;

    for (int i = 0; i < 3; i++) {
        
        gl_Position = explode(gl_in[i].gl_Position, normal, t,  mag);
       
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
