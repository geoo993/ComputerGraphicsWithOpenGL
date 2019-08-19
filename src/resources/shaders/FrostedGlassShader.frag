#version 400 core

// http://www.geeks3d.com/20101228/shader-library-frosted-glass-post-processing-shader-glsl/2/

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

uniform float coverage;    // x position of the vertical red line
uniform float randomFactor = 0.05f;  // effect param, default value: 0.05
uniform float randomScale = 5.1f;    // effect param, default value: 5.1

vec2 v1 = vec2(92.0f, 80.0f);  // effect param, default value: vec2(92.,80.)
vec2 v2 = vec2(41.0f, 62.0f);  // effect param, default value: vec2(41.,62.)

float rand(vec2 co)
{
    return fract( sin( dot( co.xy ,v1) ) + cos( dot( co.xy, v2) ) * randomScale);
}

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    /*   Frosted Glass Shader
         This is a GLSL-based post processing filter that looks like viewing through an ice-frosted glass
     */
    
    vec2 uv = fs_in.vTexCoord.xy;
    
    vec4 tc = material.guiColor;
    if (uv.x <  coverage )
    {
        vec2 rnd = vec2( rand(uv.xy), rand(uv.yx) );
        //tc = texture(material.ambientMap, uv + rnd * randomFactor).rgb;
        tc = texture(material.ambientMap, uv + rnd * randomFactor);
    }
    else if (uv.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    }
    else {
       
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
}
