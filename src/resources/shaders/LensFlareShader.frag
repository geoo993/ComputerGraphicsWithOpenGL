#version 400 core
// https://github.com/john-chapman/GfxSamples

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
    samplerCube shadowMap;          // 10.  shadow cube map
    sampler2D depthMap;             // 11.  depth map
    sampler2D noiseMap;             // 12.  noise map
    sampler2D maskMap;              // 13.  mask map
    sampler2D lensMap;              // 14.  lens map
    samplerCube cubeMap;            // 15.  sky box or environment mapping cube map
    vec4 color;
    vec4 guiColor;
    float shininess;
} material;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

uniform bool bUseDirt;
uniform float globalBrightness, starburstOffset;
uniform mat3 lensStarMatrix; // transforms texcoords
uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    vec4 sceneColor = texture(material.ambientMap, uv);
    vec4 lensDirt = texture(material.noiseMap, uv);
    vec4 lensFlare = texture(material.lensMap, uv);
    
    if (uv.x < (  coverage  ) )
    {
        
        // add lens dirt, as a further enhancement, we can use a starburst texture in addition to the lens dirt:
        if (bUseDirt) {
            
            vec2 lensStarTexcoord = (lensStarMatrix * vec3(uv, 1.0)).xy;
            vec4 lensStarburst = texture(material.glossinessMap, lensStarTexcoord);
            lensFlare *= lensDirt + lensStarburst;
        }
        tc = sceneColor + lensFlare;
    }
    else if ( uv.x  >=  (  coverage  +   0.003f) )
    {
        tc = sceneColor;
    }
    else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = sceneColor;
        }
    }
    
    vOutputColour = tc;
    
}
