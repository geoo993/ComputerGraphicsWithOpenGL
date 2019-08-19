#version 400 core
// https://www.shadertoy.com/view/lsXSWl
// https://hackernoon.com/converting-shaders-from-shadertoy-to-threejs-fe17480ed5c6

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

uniform float time;
uniform float coverage;        // between (0.0f and 1.0f)

float rng2(vec2 seed)
{
    return fract(sin(dot(seed * floor(time * 12.0f), vec2(127.1f,311.7f))) * 43758.5453123f);
}

float rng(float seed)
{
    return rng2(vec2(seed, 1.0f));
}

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    vec2 uv = fs_in.vTexCoord.xy;// / resolution.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        vec2 blockS = floor(uv * vec2(24.0f, 9.0f));
        vec2 blockL = floor(uv * vec2(8.0f, 4.0f));
        
        float r = rng2(uv);
        vec3 noise = (vec3(r, 1.0f - r, r / 2.0f + 0.5f) * 1.0f - 2.0f) * 0.08f;
        
        float lineNoise = pow(rng2(blockS), 8.0f) * pow(rng2(blockL), 3.0f) - pow(rng(7.2341f), 17.0f) * 2.0f;
        
        vec4 col1 = texture(material.ambientMap, uv);
        vec4 col2 = texture(material.ambientMap, uv + vec2(lineNoise * 0.05f * rng(5.0f), 0));
        vec4 col3 = texture(material.ambientMap, uv - vec2(lineNoise * 0.05f * rng(31.0f), 0));
        
        tc = vec4(vec3(col1.x, col2.y, col3.z) + noise, 1.0f);
        
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
