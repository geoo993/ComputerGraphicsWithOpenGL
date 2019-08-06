#version 400 core

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

uniform float coverage;        // between (0.0f and 1.0f)
const float offset = 1.0f / 300.0f;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
 
    /*   https://learnopengl.com/#!Advanced-OpenGL/Framebuffers
         This kernel highlights all edges and darkens the rest, which is quite useful when we only care about edges in an image.
     */
    
    vec2 offsets[9] = vec2[](
                             vec2(-offset,  offset), // top-left
                             vec2( 0.0f,    offset), // top-center
                             vec2( offset,  offset), // top-right
                             vec2(-offset,  0.0f),   // center-left
                             vec2( 0.0f,    0.0f),   // center-center
                             vec2( offset,  0.0f),   // center-right
                             vec2(-offset, -offset), // bottom-left
                             vec2( 0.0f,   -offset), // bottom-center
                             vec2( offset, -offset)  // bottom-right
                             );
    
    
    float kernel[9] = float[](
                              -1, -1, -1,
                              -1,  8.0f, -1,
                              -1, -1, -1
                              );
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(material.ambientMap, uv + offsets[i]));
        }
        
        vec3 color = vec3(0.0);
        for(int i = 0; i < 9; i++) {
            color += sampleTex[i] * kernel[i];
        }
        
        tc = vec4(color, 1.0f);
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
