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

uniform float coverage;        // between (0.0f and 1.0f)
uniform vec4 blurColor;
uniform int width, height;

out vec4 vOutputColour;  // formely  gl_FragColor

uniform sampler2D sampler0;

void main()
{
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = vec4(material.color, 1.0f);
    
    if (uv.x < (  coverage  ) )
    {
        float dx = 1.0 / float(width);
        float dy = 1.0 / float(height);
        int halfSize = 3;
        vec4 avg = vec4(0);
        for (int y = -halfSize; y <= halfSize; y++) {
            for (int x = -halfSize; x <= halfSize; x++) {
                avg += texture(material.ambientMap, uv+vec2(x*dx, y*dy));
            }
        }
        int N = (2*halfSize+1);
        avg /= N*N;
        tc = avg * blurColor;
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