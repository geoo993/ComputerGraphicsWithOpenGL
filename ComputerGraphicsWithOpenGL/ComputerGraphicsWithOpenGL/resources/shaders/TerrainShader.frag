#version 400 core

/*   Fragment shader variables
     
     Within the fragment shader we also have access to some interesting variables. GLSL gives us two interesting input variables called gl_FragCoord and gl_FrontFacing.
 
     gl_FragCoord
     We've seen the gl_FragCoord a couple of times during the discussion of depth testing, because the z component of the gl_FragCoord vector is equal to the depth value of that particular fragment. However, we can also use the x and y component of the vector for some interesting effects.
 
 */

// Structure holding material information:  its ambient, diffuse, specular, etc...
uniform struct Material
{
    sampler2D ambientMap;           // 0.   ambient map (albedo map)
    sampler2D diffuseMap;           // 1.   diffuse map (metallic map)
    sampler2D specularMap;          // 2.   specular map (roughness map)
    sampler2D normalMap;            // 3.   normal map
    sampler2D heightMap;            // 4.   height map
    sampler2D emissionMap;          // 5.   emission map
    sampler2D displacementMap;      // 6.   displacment map
    sampler2D aoMap;                // 7.   ambient oclusion map
    sampler2D glossinessMap;        // 8.   glossiness map
    sampler2D opacityMap;           // 9.   opacity map
    sampler2D reflectionMap;        // 10.  reflection map
    sampler2D depthMap;             // 11.  depth map
    sampler2D noiseMap;             // 12.  noise map
    sampler2D maskMap;              // 13.  mask map
    sampler2D lensMap;              // 14.  lens map
    samplerCube cubeMap;            // 15.  sky box or environment mapping cube map
    vec3 color;
    float shininess;
    bool bUseAO;
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
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{
    
    // Get the texel colour from the texture sampler
    vec4 vTexColour0 = texture(material.ambientMap, fs_in.vTexCoord);
    vec4 vTexColour1 = texture(material.normalMap, fs_in.vTexCoord);
    vec4 vTexColour2 = texture(material.diffuseMap, fs_in.vTexCoord);
    vec4 vTexColour3 = texture(material.specularMap, fs_in.vTexCoord);
    
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

        vTexColour = vTexColour2;
        
        if (bUseTexture) {
            vOutputColour = vTexColour;
        }else{
            vec3 vColour = normalize(fs_in.vWorldNormal);
            vOutputColour = vec4(vColour, 1.0f);
        }
    }
    
    // Retrieve bright parts
    float brightness = dot(vOutputColour.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f) {
        vBrightColor = vec4(vOutputColour.rgb, 1.0f);
    } else {
        vBrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // store the fragment position vector in the first gbuffer texture
    vPosition = material.bUseAO ? fs_in.vEyePosition.xyz : fs_in.vWorldPosition;
    // also store the per-fragment normals into the gbuffer
    vNormal = normalize(fs_in.vWorldNormal);
    // and the diffuse per-fragment color
    vAlbedoSpec.rgb = material.bUseAO ? vec3(0.95f) : texture(material.diffuseMap, fs_in.vTexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    vAlbedoSpec.a = material.bUseAO ? 1.0f : texture(material.specularMap, fs_in.vTexCoord).r;
    
}
