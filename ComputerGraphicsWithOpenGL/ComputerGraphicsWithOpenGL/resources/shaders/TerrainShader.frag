#version 400 core

/*   Fragment shader variables
     
     Within the fragment shader we also have access to some interesting variables. GLSL gives us two interesting input variables called gl_FragCoord and gl_FrontFacing.
 
     gl_FragCoord
     We've seen the gl_FragCoord a couple of times during the discussion of depth testing, because the z component of the gl_FragCoord vector is equal to the depth value of that particular fragment. However, we can also use the x and y component of the vector for some interesting effects.
 
 */

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
uniform struct MaterialInfo
{
    sampler2D ambientMap;  // ambient map
    sampler2D normalMap;   // normal map
    sampler2D diffuseMap;  // diffuse map
    sampler2D specularMap; // specular map
} material;

uniform float fMinHeight, fMaxHeight;
uniform bool bUseHeightMap, bUseTexture;

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
    
    // Get the texel colour from the texture sampler
    vec4 vTexColour0 = texture(material.ambientMap, fs_in.vTexCoord);
    vec4 vTexColour1 = texture(material.normalMap, fs_in.vTexCoord);
    vec4 vTexColour2 = texture(material.diffuseMap, fs_in.vTexCoord);
    vec4 vTexColour3 = texture(material.specularMap, fs_in.vTexCoord);
    
    vec3 vColour = normalize(fs_in.vEyeNormal);
    
    vec4 vTexColour;

    if (bUseHeightMap == true){

        float f = clamp(3.0f * (fs_in.vLocalPosition.y - fMinHeight) / (fMaxHeight - fMinHeight), 0.0f, 3.0f);

        if (f < 1.0f){
            vTexColour = mix(vTexColour0, vTexColour1, f);
        }else if (f < 2.0f){
            vTexColour = mix(vTexColour1, vTexColour2, f - 1.0f);
        }else{
            vTexColour = mix(vTexColour2, vTexColour3, f - 2.0f);
        }
        vOutputColour = vTexColour;
    } else {

        vTexColour = vTexColour0;
        
        if (bUseTexture) {
            vOutputColour = vTexColour;
        }else{
            vOutputColour = vec4(vColour, 1.0f);
        }
    }
    
}
