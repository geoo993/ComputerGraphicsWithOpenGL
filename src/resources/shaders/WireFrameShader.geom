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

noperspective out vec3 GEdgeDistance;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


void main()
{
    
    float a = length(gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz);
    float b = length(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    float c = length(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    
    float alpha = acos( (b*b + c*c - a*a) / (2.0f*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0f*a*c) );
    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );
    
    for (int i = 0; i < 3; i++) {
        
        gl_Position = gl_in[i].gl_Position;
        
        gs_out.vTexCoord = gs_in[i].vTexCoordPass;
        gs_out.vLocalPosition = gs_in[i].vLocalPositionPass;
        gs_out.vLocalNormal = gs_in[i].vLocalNormalPass;
        gs_out.vWorldPosition = gs_in[i].vWorldPositionPass;
        gs_out.vWorldNormal = gs_in[i].vWorldNormalPass;
        gs_out.vWorldTangent = gs_in[i].vWorldTangentPass;
        gs_out.vEyePosition = gs_in[i].vEyePositionPass;
        
        if (i == 0)GEdgeDistance = vec3( ha, 0.0f, 0.0f );
        else if (i == 1)GEdgeDistance = vec3( 0.0f, hb, 0.0f );
        else if (i == 2)GEdgeDistance = vec3( 0.0f, 0.0f, hc );
        
        EmitVertex();
    }
    
    EndPrimitive();
    
}

