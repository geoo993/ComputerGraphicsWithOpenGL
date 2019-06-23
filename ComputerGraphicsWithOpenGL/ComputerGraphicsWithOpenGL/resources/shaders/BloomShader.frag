#version 400 core

////https://www.youtube.com/watch?v=LyoSSoYyfVU
///https://learnopengl.com/Advanced-Lighting/Bloom

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
    vec4 color;
    float shininess;
} material;

uniform struct HRDLight
{
    float exposure;
    float gamma;
    bool bHDR;
} R_hrdlight;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
} fs_in;

uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{

    vec4 hdrColor = texture(material.ambientMap, fs_in.vTexCoord);
    vec4 blurColor = texture(material.depthMap, fs_in.vTexCoord);
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.color;
    
    if (uv.x <  coverage )
    {
        hdrColor += blurColor; // additive blending
        
        // HDR
        if(R_hrdlight.bHDR)
        {
            // reinhard
            // vec3 result = hdrColor / (hdrColor + vec3(1.0f));
            // tone mapping with exposure
            vec3 result = vec3(1.0f) - exp(-hdrColor.rgb * R_hrdlight.exposure);
            // also gamma correct while we're at it
            result = pow(result, vec3(1.0f / R_hrdlight.gamma));
            tc = vec4(result, 1.0f);
        } else {
            vec3 result = pow(hdrColor.rgb, vec3(1.0f / R_hrdlight.gamma));
            tc = vec4(result, 1.0f);
        }
       //tc = hdrColor + blurColor * exposure;
    } else if (uv.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    } else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
}
