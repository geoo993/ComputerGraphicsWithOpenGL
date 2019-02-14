#version 400 core

// http://www.geeks3d.com/20091027/shader-library-posterization-post-processing-effect-glsl/

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

uniform float gamma, numColors, coverage;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    /* Posterization
     
     Image posterization is the tranformation of an image with a continuous gradation of tone to several regions of fewer tones, with abrupt changes from one tone to another.
     
     */
    
    
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = vec4( material.color, 1.0f);
    if (uv.x <  coverage )
    {
        vec3 c = texture(material.ambientMap, uv).rgb;
        c = pow(c, vec3(gamma, gamma, gamma));
        c = c * numColors;
        c = floor(c);
        c = c / numColors;
        c = pow(c, vec3( 1.0f / gamma));
        tc = vec4(c, 1.0f);
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
