#version 400 core

// http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/4/

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
    vec4 vPosDevSpace;
} fs_in;

uniform float lensRadius = 3.5f;
uniform float signcurvature = 10.0f;
uniform float coverage;

#define EPSILON 0.000011

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
  
    /*  Another Very Cool Fish Eye Shader
      
     */
    
    vec2 st = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (st.x < (  coverage  ) )
    {
        float curvature = abs(signcurvature);
        float extent = lensRadius;
        float optics = extent / log2(curvature * extent + 1.0f) / 1.4427f;
        vec4 PP = fs_in.vPosDevSpace - vec4 (1.5f, 1.5f, 0.0f, 1.0f);
        float P0 = PP[0];
        float P1 = PP[1];
        float radius = sqrt(P0 * P0 + P1 * P1);
        
        float cosangle = P0 / radius;
        float sinangle = P1 / radius;
        
        float rad1, rad2, newradius;
        rad1 = (exp2((radius / optics) * 1.4427f) - 1.0f) / curvature;
        rad2 = optics * log2(1.0f + curvature * radius) / 1.4427f;
        newradius = signcurvature > 0.0f ? rad1 : rad2;
        
        vec4 FE = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        FE[0] = newradius * cosangle + 0.5f;
        FE[1] = newradius * sinangle + 0.5f;
        FE = radius <= extent ? FE : fs_in.vPosDevSpace;
        FE = curvature < EPSILON ? fs_in.vPosDevSpace : FE;
        
        tc = texture(material.ambientMap, vec2(FE));
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
