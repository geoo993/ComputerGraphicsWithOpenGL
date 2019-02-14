#version 400 core

// http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/

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

uniform float radius;  // radius between 0.0f and 1.0f
uniform float coverage;
const float PI = 3.1415926535f;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
  
    /* Fish Eye Shader
         This fish eye shader is based on the following links:

         Computer Generated Angular Fisheye Projections (http://paulbourke.net/dome/fisheye/)
     
     */
    
  
    
    
    vec2 st = fs_in.vTexCoord.xy;
    vec4 tc = vec4(material.color, 1.0f);
    
    if (st.x < (  coverage  ) )
    {
        float aperture = 178.0f;
        float apertureHalf = 0.5f * aperture * (PI / 180.0f);
        float maxFactor = sin( apertureHalf );
        
        vec2 uv;
        vec2 xy = 2.0f * st - 1.0f;
        float d = length( xy );
        if ( d < ( ( 2.0f - maxFactor ) * radius ) )
        {
            d = length( xy * maxFactor );
            float z = sqrt(1.0f - d * d);
            float r = atan(d, z) / PI;
            float phi = atan(xy.y , xy.x);
            
            uv.x = r * cos( phi ) + 0.5f;
            uv.y = r * sin( phi ) + 0.5f;
        }
        else
        {
            uv = fs_in.vTexCoord.xy;
        }
        vec4 c = texture(material.ambientMap, uv);
        tc = c;
        
    }
    else if ( st.x  >=  (  coverage  +   0.003f) )
    {
        tc = texture(material.ambientMap, st);
    }
    else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, st);
        }
    }
    
    vOutputColour = tc;
    
}
