#version 400 core

// https://learnopengl.com/#!Advanced-OpenGL/Advanced-GLSL
/*       Vertex shader variables
     We've already seen gl_Position which is the clip-space output position vector of the vertex shader. Setting gl_Position in the vertex shader is a strict requirement if you want to render anything on the screen. Nothing we haven't seen before.
 
     gl_PointSize
     One of the render primitives we're able to choose from is GL_POINTS in which case each single vertex is a primitive and rendered as a point. It is possible to set the size of the points being rendered via OpenGL's glPointSize function, but we can also influence this value in the vertex shader.
 
     An output variable defined by GLSL is called gl_PointSize that is a float variable where you can set the point's width and height in pixels. By describing the point's size in the vertex shader you can influence this point value per vertex.
 
     Influencing the point sizes in the vertex shader is disabled by default, but if you want to enable this you'll have to enable OpenGL's GL_PROGRAM_POINT_SIZE:
         glEnable(GL_PROGRAM_POINT_SIZE);  
 
     You can imagine that varying the point size per vertex is interesting for techniques like particle generation.
 
     gl_VertexID
     The gl_Position and gl_PointSize are output variables since their value is read as output from the vertex shader; we can influence the result by writing to them. The vertex shader also gives us an interesting input variable, that we can only read from, called gl_VertexID.
 
     The integer variable gl_VertexID holds the current ID of the vertex we're drawing. When doing indexed rendering (with glDrawElements) this variable holds the current index of the vertex we're drawing. When drawing without indices (via glDrawArrays) this variable holds the number of the currently processed vertex since the start of the render call.
 
     Although not particularly useful right now, it's good to know that we have access to info like this.
 
 
 */

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

/*  Interface blocks
     So far, every time we wanted to send data from the vertex to the fragment shader we declared several matching input/output variables. Declaring these one at a time is the easiest way to send data from one shader to another, but as applications become larger you probably want to send more than a few variables over which may include arrays and/or structs.
 
     To help us organize these variables GLSL offers us something called interface blocks that allows us to group together those variables. The declaration of such an interface block looks a lot like a struct declaration, except that it is now declared using an in or out keyword based on the block being an input or an output block.
 

 
 */

out VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} vs_out;

// This is the entry point into the vertex shader
void main()
{	
    
    vec4 position = vec4(inPosition, 1.0f);
    
    // Pass through the texture coordinate
    vs_out.vTexCoord = inCoord;
    
    // Get the vertex normal and vertex position in eye coordinates
    //mat3 normalMatrix = mat3(transpose(inverse(matrices.modelMatrix)));
    vs_out.vWorldNormal = matrices.normalMatrix * inNormal;
    vs_out.vLocalNormal = inNormal;
    
    vs_out.vEyePosition = matrices.viewMatrix * matrices.modelMatrix * position;
    vs_out.vWorldPosition = vec3(matrices.modelMatrix * position);
    vs_out.vLocalPosition = inPosition;
    
    // Transform the vertex spatial position using
    gl_Position = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix * position;
} 
