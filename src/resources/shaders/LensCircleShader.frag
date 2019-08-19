#version 400 core

// http://www.geeks3d.com/20091020/shader-library-lens-circle-post-processing-effect-glsl/

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

uniform float coverage;
uniform vec3 lensColor;

vec2 lensRadius = vec2( 0.45f, 0.38f ); // horizontol offset , and vertical offset
vec2 center = vec2(0.5f, 0.5f);

out vec4 vOutputColour;        // The output colour formely  gl_FragColor


void main()
{
    
    /*
     This simple shader adds a lens circle effect to the final scene image. The core of this effect is the smoothstep() function:
     
     smoothstep(outer, inner, dist);
     How this function works? If dist is greater than outer, smoothstep returns 0. If dist is lesser than inner, smoothstep returns 1. Between outer and inner, smoothstep returns an interpolated value between 0.0 and 1.0.
     
     */
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    if (uv.x <  coverage )
    {
        vec4 color = texture(material.ambientMap, uv);
        float dist = distance(uv, center);
        color.rgb *= smoothstep(lensRadius.x, lensRadius.y, dist);
        tc = color * vec4(lensColor, 1.0f);
    }
    else if (uv.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    } else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
}
