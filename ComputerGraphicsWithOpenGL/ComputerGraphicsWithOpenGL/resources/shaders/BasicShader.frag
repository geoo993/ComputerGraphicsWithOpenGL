#version 400 core

/*   Fragment shader variables
     
     Within the fragment shader we also have access to some interesting variables. GLSL gives us two interesting input variables called gl_FragCoord and gl_FrontFacing.
 
     gl_FragCoord
     We've seen the gl_FragCoord a couple of times during the discussion of depth testing, because the z component of the gl_FragCoord vector is equal to the depth value of that particular fragment. However, we can also use the x and y component of the vector for some interesting effects.
 
 */

uniform sampler2D sampler0;
uniform bool bUseTexture;  

/*
     Then we also need to declare an input interface block in the next shader which is the fragment shader. The block name (VS_OUT) should be the same in the fragment shader, but the instance name (vs_out as used in the vertex shader) can be anything we like - avoiding confusing names like vs_out that actually contains input variables.
 
     As long as both interface block names are equal, their corresponding input and output is matched together. This is another useful feature that helps organize your code and proves useful when crossing between certain shader stages like the geometry shader.
 
 */
in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vEyePosition;
    vec3 vEyeNormal;
} fs_in;


out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    vec4 vTexColour = texture(sampler0, fs_in.vTexCoord); 
    vec3 vColour = normalize(fs_in.vEyeNormal);
    
    if (bUseTexture){
        vOutputColour = vTexColour; 
    }else{
        vOutputColour = vec4(vColour, 1.0f);
    }
    
}
