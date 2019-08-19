#version 400 core
// https://www.shadertoy.com/view/4ddGWr

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

uniform vec2 mouse;
uniform vec4 date;          // (year, month, day, time in seconds)
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)


// for a dramatic display of the value of dithering,
// use these settings.
//const float PaletteRGBSize = 2.;// only 1 bit per R G B (8-color palette)
//const float ResolutionDivisor = 1.;// 1 pixel = 1 pixel, the finest visible.


const float PaletteRGBSize = 4.0f;// number of values possible for each R, G, B.
const float ResolutionDivisor = 2.0f;

//#define USE_BAYER4x4
#define USE_BAYER8x8
//#define USE_NOISE
//#define USE_ANIM_NOISE

//----------------------------------------------------------------------------

float quantize(float inp, float period)
{
    return floor((inp+period/2.0f)/period)*period;
}
vec2 quantize(vec2 inp, vec2 period)
{
    return floor((inp+period/2.0f)/period)*period;
}

//----------------------------------------------------------------------------
float bayer4x4(vec2 uvScreenSpace)
{
    vec2 bayerCoord = floor(uvScreenSpace/ResolutionDivisor);
    bayerCoord = mod(bayerCoord, 4.0f);
    const mat4 bayerMat = mat4(
                               1,9,3,11,
                               13,5,15,7,
                               4,12,2,10,
                               16,8,14,6) / 16.0f;
    int bayerIndex = int(bayerCoord.x + bayerCoord.y * 4.0f);
    if(bayerIndex == 0) return bayerMat[0][0];
    if(bayerIndex == 1) return bayerMat[0][1];
    if(bayerIndex == 2) return bayerMat[0][2];
    if(bayerIndex == 3) return bayerMat[0][3];
    if(bayerIndex == 4) return bayerMat[1][0];
    if(bayerIndex == 5) return bayerMat[1][1];
    if(bayerIndex == 6) return bayerMat[1][2];
    if(bayerIndex == 7) return bayerMat[1][3];
    if(bayerIndex == 8) return bayerMat[2][0];
    if(bayerIndex == 9) return bayerMat[2][1];
    if(bayerIndex == 10) return bayerMat[2][2];
    if(bayerIndex == 11) return bayerMat[2][3];
    if(bayerIndex == 12) return bayerMat[3][0];
    if(bayerIndex == 13) return bayerMat[3][1];
    if(bayerIndex == 14) return bayerMat[3][2];
    if(bayerIndex == 15) return bayerMat[3][3];
    return 10.0f;// impossible
}

float bayer8x8(sampler2D tex, vec2 uvScreenSpace)
{
    return texture(tex, uvScreenSpace/(ResolutionDivisor * 8.0f)).r;
}

vec3 getSceneColor(sampler2D tex, vec2 uv)
{
    return texture(tex, uv).rgb;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898f,78.233f))) * 43758.5453f);
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
    vec4 vTexColour = texture(material.ambientMap, fs_in.vTexCoord);
    
    vec2 uv = fragCoordViewportCoordinates / resolution.xy;// 0-1
    
    // space between values of the dest palette
    vec3 quantizationPeriod = vec3(1.0f/(PaletteRGBSize-1.0f));
    
    vec2 uvPixellated = floor(fragCoordViewportCoordinates / ResolutionDivisor) * ResolutionDivisor;
    
    // original color panel---------------------
    vec3 originalCol = getSceneColor(material.ambientMap, uv);
    
    // dithered panel---------------------
    vec3 dc = getSceneColor(material.ambientMap, uvPixellated / resolution.xy);
    // apply bayer matrix, perturbing the original color values.
#ifdef USE_BAYER4x4
    dc += (bayer4x4(fragCoordViewportCoordinates)-0.5f)*(quantizationPeriod);
#endif
#ifdef USE_BAYER8x8
    dc += (bayer8x8(material.noiseMap, fragCoordViewportCoordinates)-0.5f)*(quantizationPeriod);
#endif
#ifdef USE_NOISE
    dc += (rand(uvPixellated)-0.5f)*(quantizationPeriod);
#endif
#ifdef USE_ANIM_NOISE
    dc += (rand(vec2(rand(uvPixellated),date.w))-.5)*(quantizationPeriod);
#endif
    // quantize color to palette
    dc = vec3(
              quantize(dc.r, quantizationPeriod.r),
              quantize(dc.g, quantizationPeriod.g),
              quantize(dc.b, quantizationPeriod.b)
              );
    
    // quantize to palette (raw quantization panel)---------------------
    vec3 qc = getSceneColor(material.ambientMap, uvPixellated / resolution.xy);
    qc = vec3(
              quantize(qc.r, quantizationPeriod.r),
              quantize(qc.g, quantizationPeriod.g),
              quantize(qc.b, quantizationPeriod.b)
              );

    
    // framing and post
    float ySplit = ((mouse.y > 0.0f) ? (1.0f - (mouse.y / resolution.y)) : 1.0f);
    float xSplit = coverage;
    if(uv.x > xSplit) {
        vOutputColour = vec4(originalCol, 1.0f);
    } else {
        if(uv.y > ySplit) {
            vOutputColour = vec4(dc, 1.0f);
        } else {
            vOutputColour = vec4(qc, 1.0f);
        }
    }
    
    float f = abs(uv.x - xSplit);
    vOutputColour.rgb *= smoothstep(0.00f, 0.005f, f);
    f = abs(uv.y - ySplit);
    if(uv.x < xSplit) {
        vOutputColour.rgb *= smoothstep(0.00f, 0.005f, f);
    }
}
