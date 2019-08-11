#version 400 core
//https://www.shadertoy.com/view/XdXXzX
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

uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{

    vec2 uv = fs_in.vTexCoord.xy - 0.5f;
    vec4 tc = material.guiColor;
    
    if (fs_in.vTexCoord.x <  coverage )
    {
        const vec2 M = vec2(0.36f, 0.196f);
        uv = vec2(uv.x*uv.x - uv.y*uv.y, 2.0f * uv.x * uv.y) + M;
        uv = vec2(uv.x*uv.x - uv.y*uv.y, 2.0f * uv.x * uv.y) + M;
        uv = vec2(uv.x*uv.x - uv.y*uv.y, 2.0f * uv.x * uv.y) + M;
        
        tc = vec4(texture(material.ambientMap,uv));
        
    } else if (fs_in.vTexCoord.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, fs_in.vTexCoord.xy);
    } else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, fs_in.vTexCoord.xy);
        }
    }
    
    vOutputColour = tc;
}
