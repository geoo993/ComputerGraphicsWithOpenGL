#version 400 core

out VS_OUT
{
    vec4 vWorldPosition;
} gs_out;

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

void main()
{
    
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            gs_out.vWorldPosition = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * gs_out.vWorldPosition;
            EmitVertex();
        }
        EndPrimitive();
    }

}
