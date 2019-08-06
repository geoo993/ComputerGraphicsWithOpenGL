#version 400 core

// http://www.geeks3d.com/20091009/shader-library-night-vision-post-processing-filter-glsl/

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

uniform float elapsedTime; // seconds
uniform float luminanceThreshold = 0.2f;
uniform float colorAmplification = 4.0f;
uniform float coverage;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    vec4 tc = material.guiColor;
    
    // Set effectCoverage to 1.0 for normal use.
    if (fs_in.vTexCoord.x < coverage)
    {
        vec2 uv;
        uv.x = 0.4f * sin(elapsedTime * 50.0f);
        uv.y = 0.4f * cos(elapsedTime * 50.0f);
        float m = texture(material.maskMap, fs_in.vTexCoord).r;  // maskTex
        vec3 n = texture(material.noiseMap, (fs_in.vTexCoord * 3.5f) + uv).rgb; // noiseTex
        vec3 c = texture(material.ambientMap, fs_in.vTexCoord + (n.xy * 0.005f )).rgb;
        
        float lum = dot( vec3(0.30f, 0.59f, 0.11f), c);
        if (lum < luminanceThreshold){
            c *= colorAmplification;
        }
        
        vec3 visionColor = vec3(0.1f, 0.95f, 0.2f);
        tc.rgb = (c + (n * 0.2f) ) * visionColor * m;
    } else if (fs_in.vTexCoord.x >= ( coverage + 0.003f) )
    {
       tc = texture(material.ambientMap, fs_in.vTexCoord);
    }
    else {
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, fs_in.vTexCoord);
        }
    }
    
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    result.rgb = tc.rgb;
    result.a = 1.0f;
    vOutputColour = result;
}
