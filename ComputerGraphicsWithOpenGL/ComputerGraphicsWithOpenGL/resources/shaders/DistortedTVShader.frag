#version 400 core
// https://www.shadertoy.com/view/ldXGW4
// https://hackernoon.com/converting-shaders-from-shadertoy-to-threejs-fe17480ed5c6

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

uniform float time;
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

// change these values to 0.0 to turn off individual effects
float vertJerkOpt = 1.0;
float vertMovementOpt = 1.0;
float bottomStaticOpt = 1.0;
float scalinesOpt = 1.0;
float rgbOffsetOpt = 1.0;
float horzFuzzOpt = 1.0;

// Noise generation functions borrowed from:
// https://github.com/ashima/webgl-noise/blob/master/src/noise2D.glsl

vec3 mod289(vec3 x) {
    return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec2 mod289(vec2 x) {
    return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec3 permute(vec3 x) {
    return mod289(((x*34.0f)+1.0f)*x);
}

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187f,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439f,  // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626f,  // -1.0 + 2.0 * C.x
                        0.024390243902439f); // 1.0 / 41.0
    // First corner
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);
    
    // Other corners
    vec2 i1;
    //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
    //i1.y = 1.0 - i1.x;
    i1 = (x0.x > x0.y) ? vec2(1.0f, 0.0f) : vec2(0.0f, 1.0f);
    // x0 = x0 - 0.0 + 0.0 * C.xx ;
    // x1 = x0 - i1 + 1.0 * C.xx ;
    // x2 = x0 - 1.0 + 2.0 * C.xx ;
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    
    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    vec3 p = permute( permute( i.y + vec3(0.0f, i1.y, 1.0f ))
                     + i.x + vec3(0.0f, i1.x, 1.0f ));
    
    vec3 m = max(0.5f - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0f);
    m = m*m ;
    m = m*m ;
    
    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
    
    vec3 x = 2.0f * fract(p * C.www) - 1.0f;
    vec3 h = abs(x) - 0.5f;
    vec3 ox = floor(x + 0.5f);
    vec3 a0 = x - ox;
    
    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    m *= 1.79284291400159f - 0.85373472095314f * ( a0*a0 + h*h );
    
    // Compute final noise value at P
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0f * dot(m, g);
}

float staticV(vec2 uv) {
    float staticHeight = snoise(vec2(9.0f,time*1.2f+3.0f))*0.3f+5.0f;
    float staticAmount = snoise(vec2(1.0f,time*1.2f-6.0f))*0.1f+0.3f;
    float staticStrength = snoise(vec2(-9.75f,time*0.6f-3.0f))*2.0f+2.0f;
    return (1.0f-step(snoise(vec2(5.0f*pow(time,2.0f)+pow(uv.x*7.0f,1.2f),pow((mod(time,100.0f)+100.0f)*uv.y*0.3f+3.0f,staticHeight))),staticAmount))
    *staticStrength;
}


out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    // https://gamedev.stackexchange.com/questions/106674/shadertoy-getting-help-moving-to-glsl
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml
    // https://stackoverflow.com/questions/26965787/how-to-get-accurate-fragment-screen-position-like-gl-fragcood-in-vertex-shader
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 fragCoordViewportCoordinates = fragCoord * 0.5f + 0.5f;
    vec2 resolution = vec2(width, height);// width and height of the screen
    vec2 uv = fs_in.vTexCoord.xy;
    //vec2 uv =  fragCoordViewportCoordinates.xy/resolution.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        float jerkOffset = (1.0f-step(snoise(vec2(time*1.3f,5.0f)),0.8f))*0.05f;
        
        float fuzzOffset = snoise(vec2(time*15.0f,uv.y*80.0f))*0.003f;
        float largeFuzzOffset = snoise(vec2(time*1.0f,uv.y*25.0f))*0.004f;
        
        float vertMovementOn = (1.0f-step(snoise(vec2(time*0.2f,8.0f)),0.4f))*vertMovementOpt;
        float vertJerk = (1.0f-step(snoise(vec2(time*1.5f,5.0f)),0.6f))*vertJerkOpt;
        float vertJerk2 = (1.0f-step(snoise(vec2(time*5.5f,5.0f)),0.2f))*vertJerkOpt;
        float yOffset = abs(sin(time)*4.0f)*vertMovementOn+vertJerk*vertJerk2*0.3f;
        float y = mod(uv.y+yOffset,1.0f);
        
        
        float xOffset = (fuzzOffset + largeFuzzOffset) * horzFuzzOpt;
        
        float staticVal = 0.0f;
        
        for (float y = -1.0; y <= 1.0; y += 1.0) {
            float maxDist = 5.0f/200.0f;
            float dist = y/200.0;
            staticVal += staticV(vec2(uv.x,uv.y+dist))*(maxDist-abs(dist))*1.5f;
        }
        
        staticVal *= bottomStaticOpt;
        
        float red     =   texture(    material.ambientMap,     vec2(uv.x + xOffset -0.01f*rgbOffsetOpt,y)).r+staticVal;
        float green =     texture(    material.ambientMap,     vec2(uv.x + xOffset,      y)).g+staticVal;
        float blue     =    texture(    material.ambientMap,     vec2(uv.x + xOffset +0.01f*rgbOffsetOpt,y)).b+staticVal;
        
        vec3 color = vec3(red,green,blue);
        float scanline = sin(uv.y*800.0f)*0.04f*scalinesOpt;
        color -= scanline;
        
        tc = vec4(color,1.0f);
    } else if (uv.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    } else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
    
}
