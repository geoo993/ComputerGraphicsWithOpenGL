#version 400 core
#define PI 3.1415927
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

uniform float radius = 0.9f;  // radius between 0.0f and 1.0f
uniform float coverage;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
  
    /* Fish Eye Shader
         This fish eye shader is based on the following links:

         Computer Generated Angular Fisheye Projections (http://paulbourke.net/dome/fisheye/)
     
     */
    
    vec2 st = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (st.x < (  coverage  ) )
    {
        float aperture = 178.0f;
        float apertureHalf = 0.5f * aperture * (float(PI) / 180.0f);
        float maxFactor = sin( apertureHalf );
        
        vec2 uv;
        vec2 xy = 2.0f * st - 1.0f;
        float d = length( xy );
        if ( d < ( ( 2.0f - maxFactor ) * radius ) )
        {
            d = length( xy * maxFactor );
            float z = sqrt(1.0f - d * d);
            float r = atan(d, z) / float(PI);
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
