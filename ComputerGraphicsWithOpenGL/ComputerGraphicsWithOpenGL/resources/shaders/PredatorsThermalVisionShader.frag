#version 400 core

// http://www.geeks3d.com/20101123/shader-library-predators-thermal-vision-post-processing-filter-glsl/

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
    samplerCube cubeMap;            // 14.  sky box or environment mapping cube map
    vec3 color;
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

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    /*  Predator’s Thermal Vision
     Here is a new post FX shader, that shader shows how to implement a Predator-like thermal vision post processing effect in GLSL
     
     
     */
    
    vec2 uv = fs_in.vTexCoord.xy;
    
    vec3 tc = material.color;
    
    if (uv.x < (  coverage  ) )
    {
        vec3 pixcol = texture(material.ambientMap, uv).rgb;
        vec3 colors[3] = vec3[](
                                  vec3(0.0f, 0.0f, 1.0f),
                                  vec3(1.0f, 1.0f, 0.0f),
                                  vec3(1.0f, 0.0f, 0.0f)
                                  );
        
        float lum = (pixcol.r + pixcol.g + pixcol.b)  / 3.0f;
        int ix = (lum < 0.5f) ? 0  :  1;
        tc = mix( colors[ix],  colors[ix + 1 ],  (lum - float(ix) * 0.5f) / 0.5f);
    }
    else if ( uv.x  >=  (  coverage  +   0.003f) )
    {
        tc = texture(material.ambientMap, uv).rgb;
    }
    else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv).rgb;
        }
    }
    
    vOutputColour = vec4(tc, 1.0f);
    
}
