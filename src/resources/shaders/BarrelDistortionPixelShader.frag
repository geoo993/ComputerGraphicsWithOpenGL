#version 400 core

// http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/2/

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

uniform float barrelPower = 2.0f;
uniform float coverage;

vec2 Distort(vec2 p, float power)
{
    float theta  = atan(p.y, p.x);
    float radius = length(p);
    radius = pow(radius, power);
    p.x = radius * cos( theta );
    p.y = radius * sin( theta );
    return 0.5f * (p + 1.0f);
}


out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
  
    /*  Barrel distortion Pixel Shader
         Barrel distortion is a lens effect which causes images to be spherised or inflated.
     
         This barrel distortion demo shows a regular post processing filter that runs the filter in the pixel shader.
     
     
     */
    
    
    
    vec2 st = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (st.x < (  coverage  ) )
    {
        vec2 xy = 2.0f * fs_in.vTexCoord.xy - 1.0f;
        vec2 uv;
        
        float d = length(xy);
        
        if (d < 1.0f)
        {
            uv = Distort(xy, barrelPower);
        }
        else
        {
            uv = fs_in.vTexCoord.xy;
        }
        
        tc = texture(material.ambientMap, uv);
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
