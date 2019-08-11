#version 400 core

/*
 Bloom effect based on:
 https://learnopengl.com/#!Advanced-Lighting/Bloom
 http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
 25/03/2017
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

uniform bool bHorizontal;
uniform float intensity = 1.0f;
uniform float coverage;        // between (0.0f and 1.0f)
float weight[5] = float[] (0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f);

/*
float weight[13] = float[] (
                            0.0896631113333857,
                            0.0874493212267511,
                            0.0811305381519717,
                            0.0715974486241365,
                            0.0601029809166942,
                            0.0479932050577658,
                            0.0364543006660986,
                            0.0263392293891488,
                            0.0181026699707781,
                            0.0118349786570722,
                            0.0073599963704157,
                            0.0043538453346397,
                            0.0024499299678342
                            );
*/

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main() {
    
    
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        vec2 tex_offset = 1.0f / textureSize(material.depthMap, 0); // gets size of single texel
        vec3 result = texture(material.depthMap, uv).rgb * weight[0]; // current fragment's contribution
        if(bHorizontal) {
            for(int i = 1; i < 5; ++i) {
                result += texture(material.depthMap, uv + vec2(tex_offset.x * i, 0.0f)).rgb * weight[i] * intensity;
                result += texture(material.depthMap, uv - vec2(tex_offset.x * i, 0.0f)).rgb * weight[i] * intensity;
            }
        } else {
            for(int i = 1; i < 5; ++i) {
                result += texture(material.depthMap, uv + vec2(0.0f, tex_offset.y * i)).rgb * weight[i] * intensity;
                result += texture(material.depthMap, uv - vec2(0.0f, tex_offset.y * i)).rgb * weight[i] * intensity;
            }
        }
        
        tc = vec4(result, 1.0f);
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

