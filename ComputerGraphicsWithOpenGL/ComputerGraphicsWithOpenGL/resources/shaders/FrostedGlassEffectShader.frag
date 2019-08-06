#version 400 core

// http://www.geeks3d.com/20101228/shader-library-frosted-glass-post-processing-shader-glsl/

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

uniform float width;   // width of the current render target
uniform float height;  // height of the current render target

uniform float coverage; // x position of the vertical red line
uniform float PixelX;   // effect param, default value: 2.0
uniform float PixelY;   // effect param, default value: 2.0
uniform float Freq;     // effect param, default value: 0.0115

vec4 spline(float x, vec4 c1, vec4 c2, vec4 c3, vec4 c4,
            vec4 c5, vec4 c6, vec4 c7, vec4 c8, vec4 c9)
{

    float w1, w2, w3, w4, w5, w6, w7, w8, w9;
    w1 = 0.0f;
    w2 = 0.0f;
    w3 = 0.0f;
    w4 = 0.0f;
    w5 = 0.0f;
    w6 = 0.0f;
    w7 = 0.0f;
    w8 = 0.0f;
    w9 = 0.0f;
    float tmp = x * 8.0f;
    
    if (tmp <= 1.0f) {
        w1 = 1.0f - tmp;
        w2 = tmp;
    }
    else if (tmp <= 2.0f) {
        tmp = tmp - 1.0f;
        w2 = 1.0f - tmp;
        w3 = tmp;
    }
    else if (tmp <= 3.0f) {
        tmp = tmp - 2.0f;
        w3 = 1.0f - tmp;
        w4 = tmp;
    }
    else if (tmp <= 4.0f) {
        tmp = tmp - 3.0f;
        w4 = 1.0f - tmp;
        w5 = tmp;
    }
    else if (tmp <= 5.0f) {
        tmp = tmp - 4.0f;
        w5 = 1.0f - tmp;
        w6 = tmp;
    }
    else if (tmp <= 6.0f) {
        tmp = tmp - 5.0f;
        w6 = 1.0f - tmp;
        w7 = tmp;
    }
    else if (tmp <= 7.0f) {
        tmp = tmp - 6.0f;
        w7 = 1.0f - tmp;
        w8 = tmp;
    }
    else
    {
        //tmp = saturate(tmp - 7.0);
        // http://www.ozone3d.net/blogs/lab/20080709/saturate-function-in-glsl/
        
        tmp = clamp(tmp - 7.0f, 0.0f, 1.0f);
        w8 = 1.0f - tmp;
        w9 = tmp;
    }
    
    return w1 * c1 + w2 * c2 + w3 * c3 + w4 * c4 + w5 * c5 + w6 * c6 + w7 * c7 + w8 * c8 + w9 * c9;
}

vec3 NOISE2D(vec2 p)
{ 
    return texture(material.noiseMap, p).xyz;
}

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    /*   Frosted Glass Shader
         This is a GLSL-based post processing filter that looks like viewing through an ice-frosted glass
     
     */
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x < ( coverage ) )
    {
        float DeltaX = PixelX / width;
        float DeltaY = PixelY / height;
        vec2 ox = vec2( DeltaX, 0.0f );
        vec2 oy = vec2( 0.0f, DeltaY );
        vec2 PP = uv - oy;
        vec4 C00 = texture(material.ambientMap, PP - ox);
        vec4 C01 = texture(material.ambientMap, PP);
        vec4 C02 = texture(material.ambientMap, PP + ox);
        PP = uv;
        vec4 C10 = texture(material.ambientMap, PP - ox);
        vec4 C11 = texture(material.ambientMap, PP);
        vec4 C12 = texture(material.ambientMap, PP + ox);
        PP = uv + oy;
        vec4 C20 = texture(material.ambientMap, PP - ox);
        vec4 C21 = texture(material.ambientMap, PP);
        vec4 C22 = texture(material.ambientMap, PP + ox);
        
        float n = NOISE2D(Freq * uv).x;
        n = mod(n, 0.111111f) / 0.111111f;
        vec4 result = spline(n, C00, C01, C02, C10, C11, C12, C20, C21, C22);
        tc = result;
        
    }else if ( uv.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    }else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
        
    vOutputColour = tc;
    
}
