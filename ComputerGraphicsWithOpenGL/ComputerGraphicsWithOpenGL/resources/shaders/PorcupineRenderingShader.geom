#version 400 core

// https://learnopengl.com/#!Advanced-OpenGL/Geometry-Shader
// http://www.geeks3d.com/20130905/exploring-glsl-normal-visualizer-with-geometry-shaders-shader-library/
// http://sunandblackcat.com/tipFullView.php?l=eng&topicid=29&topic=Geometry-Shader-Normals-Edges

/*   Visualizing normal vectors (Porcupine rendering)
     In this section we're going to give you an example of using the geometry shader that is actually useful: visualizing the normal vectors of any object. When programming lighting shaders you will eventually run into weird visual outputs of which the cause is hard to determine. A common cause of lighting errors is due to incorrect normal vectors caused by incorrectly loading vertex data, improperly specifying them as vertex attributes or incorrectly managing them in the shaders. What we want is some way to detect if the normal vectors we supplied are correct. A great way to determine if your normal vectors are correct is by visualizing them and it just so happens that the geometry shader is an extremely useful tool for this purpose.
 
     The idea is as follows: we first draw the scene as normal without a geometry shader and then we draw the scene a second time, but this time only displaying normal vectors that we generate via a geometry shader. The geometry shader takes as input a triangle primitive and generates 3 lines from those in the direction of the normal vector - one normal vector for each vertex. In pseudocode it'll look something like this:
 
 
     shader.use();
     DrawScene();
     normalDisplayShader.use();
     DrawScene();
     This time we're creating a geometry shader that uses the vertex normals supplied by the model instead of generating them ourselves. To accommodate for scaling and rotations (due to the view and model matrix) we'll transform the normals first with a normal matrix before transforming it to clip-space coordinates (geometry shader receives its position vectors as clip-space coordinates so we should also transform the normal vectors to the same space). This can all be done in the vertex shader:
 
 */

in VS_OUT
{
    vec2 vTexCoordPass;    // Texture coordinate
    vec3 vLocalPositionPass;
    vec3 vLocalNormalPass;
    vec3 vWorldPositionPass;
    vec3 vWorldNormalPass;
    vec3 vWorldTangentPass;
    vec4 vEyePositionPass;
    mat4 vMVPPass;
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
layout (line_strip, max_vertices = 8) out;  // 6 for vertex normal, 8 for both vertex normal and face normal

void GenerateLine(int index, float magnitude, vec3 color)
{
    gl_Position = gl_in[index].gl_Position;
    gs_out.vWorldNormal = color;
    EmitVertex();
    
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].vWorldNormalPass, 0.0f) * magnitude;
    gs_out.vWorldNormal = color;
    EmitVertex();
    
    EndPrimitive();
}

void GenerateVertexNormal(int index, float magnitude, vec3 color){
    
    /*
         The generation of the vertex normals is simple. Each normal is a line made up of two vertices. The first vertex is equal to the incoming vertex (which belongs to the current mesh). The second vertex is equal to the first vertex with a displacement along the direction of the vertex normal:
     
         V0 = Pi
         V1 = Pi + (normal_length * Ni)
     
         Where i is the vertex index (0 to 2 because the input of the geometry shader is a triangle), Pi and Ni are the position and the normal of the i-th vertex. V0 and V1 are the vertices of the new line.
     */
    
    //------ 3 lines for the 3 vertex normals
    //
    vec3 P = gs_in[index].vLocalPositionPass.xyz;
    vec3 N = gs_in[index].vLocalNormalPass.xyz;
    
    gl_Position = gs_in[index].vMVPPass * vec4(P, 1.0f);
    gs_out.vWorldNormal = color;
    EmitVertex();
    
    // magnitude between 0 and 1
    gl_Position = gs_in[index].vMVPPass * vec4(P + N * magnitude, 1.0f);
    gs_out.vWorldNormal = color;
    EmitVertex();
    
    EndPrimitive();

}

void GenerateFaceNormal(int index, float magnitude, vec3 color){
    
    /*
         The generation of the vertex normal was simple. Let’s see now how to generate the face normal in the geometry shader. All we need are the thee vertices that define a triangle. We are lucky because those vertices are the inputs of the geometry shader thanks to the following line:
     
         layout(triangles) in;
         If P0, P1 and P2 are the positions of the face vertices, the face normal is the result of the following cross product:
     
         V0 = P0-P1
         V1 = P2-P1
         N = cross (V1, V0)
     */
    
    //------ One line for the face normal
    //
    vec3 P0 = gs_in[0].vLocalPositionPass.xyz;
    vec3 P1 = gs_in[1].vLocalPositionPass.xyz;
    vec3 P2 = gs_in[2].vLocalPositionPass.xyz;
    
    vec3 V0 = P0 - P1;
    vec3 V1 = P2 - P1;
    
    vec3 N = cross(V1, V0);
    N = normalize(N);
    
    // Center of the triangle
    vec3 P = (P0 + P1 +P2) / 3.0f;
    
    gl_Position = gs_in[index].vMVPPass * vec4(P, 1.0f);
    gs_out.vWorldNormal = color;
    EmitVertex();
    
    // magnitude between 0 and 1
    gl_Position = gs_in[index].vMVPPass * vec4(P + N * magnitude, 1.0f);
    gs_out.vWorldNormal = color;
    EmitVertex();
    EndPrimitive();
}

uniform float magnitude; // between 0.0f and 1.0f
uniform vec3 vertexNormalColor, faceNormalColor;

void main() {
    
    for (int i = 0; i < 3; i++) {
      
        gs_out.vTexCoord = gs_in[i].vTexCoordPass;
        gs_out.vLocalPosition = gs_in[i].vLocalPositionPass;
        gs_out.vLocalNormal = gs_in[i].vLocalNormalPass;
        gs_out.vWorldPosition = gs_in[i].vWorldPositionPass;
        gs_out.vWorldNormal = gs_in[i].vWorldNormalPass;
        gs_out.vWorldTangent = gs_in[i].vWorldTangentPass;
        gs_out.vEyePosition = gs_in[i].vEyePositionPass;
        
        
        //GenerateLine(i, magnitude, vertexNormalColor);
        GenerateVertexNormal(i, magnitude, vertexNormalColor);
        GenerateFaceNormal(i, magnitude, faceNormalColor);
        
    }
     
} 
