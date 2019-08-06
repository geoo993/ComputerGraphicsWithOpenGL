#version 400 core

// http://www.geeks3d.com/20091116/shader-library-2d-shockwave-post-processing-filter-glsl/

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
    samplerCube cubeMap;            // 14.  sky box or environment mapping cube map
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

uniform vec2 center = vec2(0.5f, 0.5f); // Mouse position (in texture coord space, between [0  1]).
uniform float time; // shockwave elapsed time in second.
uniform vec3 shockParams; // vec3(10.0, 0.8, 0.1) shockwave parameters
uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
  
    /*  2D Shockwave Shader
         This new shader shows a 2D shockwave post processing effect.
     
     */
    
    vec2 uv = fs_in.vTexCoord.xy ;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        vec2 texCoord = uv;
        float dist = distance(uv, center);
        
        if ( (dist <= (time + shockParams.z) ) && ( dist >= (time - shockParams.z) ) )
        {
            float diff = (dist - time);
            float powDiff = 1.0f - pow( abs( diff * shockParams.x ), shockParams.y);
            float diffTime = diff * powDiff;
            vec2 diffUV = normalize(uv - center);
            texCoord = uv + (diffUV * diffTime);
        }
        
        tc = texture(material.ambientMap, texCoord);
    }
    else if ( uv.x  >=  (  coverage  +   0.003f) )
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
