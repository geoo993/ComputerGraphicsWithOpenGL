#version 400 core

////https://github.com/mattdesl/lwjgl-basics/wiki/ShaderLesson5
////https://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene

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

//const float sampleDist = 1.0f;
//const float sampleStrength = 2.2f;

// RadialBlur - Shader
uniform float resolution = 2.0f;
uniform float radius = 0.4f;
uniform float coverage;        // between (0.0f and 1.0f)
out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  ) )
    {
        float samples[10];
        samples[0] = -0.08f;
        samples[1] = -0.05f;
        samples[2] = -0.03f;
        samples[3] = -0.02f;
        samples[4] = -0.01f;
        samples[5] =  0.01f;
        samples[6] =  0.02f;
        samples[7] =  0.03f;
        samples[8] =  0.05f;
        samples[9] =  0.08f;
        
        vec2 dir = 0.5f - uv;
        float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
        dir = dir/dist;
        
        vec4 color = texture(material.ambientMap, uv);
        vec4 sum = color;
        
        for (int i = 0; i < 10; i++)
            sum += texture( material.ambientMap, uv + dir * samples[i] * radius );
        
        sum *= 1.0f / 11.0f;
        float t = dist * resolution;
        t = clamp( t ,0.0f,1.0f);
        
        tc = mix( color, sum, t );
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
