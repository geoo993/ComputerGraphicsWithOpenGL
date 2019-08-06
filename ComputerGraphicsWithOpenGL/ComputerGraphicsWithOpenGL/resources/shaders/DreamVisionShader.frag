#version 400 core

// http://www.geeks3d.com/20091112/shader-library-dream-vision-post-processing-filter-glsl/

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

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    /*   Dream Vision Post Processing Effect
         This new shader shows the kind of vision one may have in a dream…
    */

    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    if (uv.x <  coverage )
    {
        vec4 c = texture(material.ambientMap, uv);
        c += texture(material.ambientMap, uv + 0.001f);
        c += texture(material.ambientMap, uv + 0.003f);
        c += texture(material.ambientMap, uv + 0.005f);
        c += texture(material.ambientMap, uv + 0.007f);
        c += texture(material.ambientMap, uv + 0.009f);
        c += texture(material.ambientMap, uv + 0.011f);
        
        c += texture(material.ambientMap, uv - 0.001f);
        c += texture(material.ambientMap, uv - 0.003f);
        c += texture(material.ambientMap, uv - 0.005f);
        c += texture(material.ambientMap, uv - 0.007f);
        c += texture(material.ambientMap, uv - 0.009f);
        c += texture(material.ambientMap, uv - 0.011f);
        
        c.rgb = vec3((c.r + c.g + c.b) / 3.0f);
        c = c / 9.5f;
        tc = c;
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
