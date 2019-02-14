#version 400 core

// http://glampert.com/2014/01-26/visualizing-the-depth-buffer/
// https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/

in vec2 vTexCoord;		

uniform sampler2D depthMap;
uniform bool isPerspective = false;
uniform float near_plane;
uniform float far_plane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0f - 1.0f; // Back to NDC 
    return (2.0f * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));    
}


/*
// 'colorImage' is a sampler2D with the depth image
// read from the current depth buffer bound to it.
//
float LinearizeDepth(in sampler2D tex, in vec2 uv)
{
    float zNear = 0.5;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 2000.0; // TODO: Replace by the zFar  of your perspective projection
    float depth = texture2D(tex, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}
*/


out vec4 vOutputColour;    

void main()
{
    /*
     Sometimes it can be quite useful to visualize the depth buffer of a rendered frame.
     Several rendering techniques such as shadow mapping and depth pre-pass rely on the depth buffer.
     It is always handy to visualize it in real time to make sure it is being written as expected.
     

     
     */
    float depthValue = texture(depthMap, vTexCoord).r;
    
    if (isPerspective){
        vOutputColour = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0f); // perspective
    }else{
        vOutputColour = vec4(vec3(depthValue), 1.0f); // orthographic
    }
}

