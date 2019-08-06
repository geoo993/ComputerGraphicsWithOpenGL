#version 400 core

// https://gamedev.stackexchange.com/questions/152496/how-can-i-make-natural-rain-drops-on-screen
// https://www.shadertoy.com/view/ltffzl
// https://www.shadertoy.com/view/XdSGDc
// https://seblagarde.wordpress.com/2012/12/27/water-drop-2a-dynamic-rain-and-its-effects/
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

uniform int mouseDown;
uniform vec2 mouse;
uniform float time;
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

#define S(a, b, t) smoothstep(a, b, t)
#define CHEAP_NORMALS
//#define HAS_HEART
#define USE_POST_PROCESSING

vec3 N13(float p) {
    //  from DAVE HOSKINS
    vec3 p3 = fract(vec3(p) * vec3(0.1031f,0.11369f,0.13787f));
    p3 += dot(p3, p3.yzx + 19.19f);
    return fract(vec3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}

vec4 N14(float t) {
    return fract(sin(t*vec4(123.0f, 1024.0f, 1456.0f, 264.0f))*vec4(6547.0f, 345.0f, 8799.0f, 1564.0f));
}
float N(float t) {
    return fract(sin(t*12345.564f)*7658.76f);
}

float Saw(float b, float t) {
    return S(0.0f, b, t)*S(1.0f, b, t);
}

vec2 DropLayer2(vec2 uv, float t) {
    vec2 UV = uv;
    
    uv.y += t*0.75f;
    vec2 a = vec2(6.0f, 1.0f);
    vec2 grid = a*2.0f;
    vec2 id = floor(uv*grid);
    
    float colShift = N(id.x);
    uv.y += colShift;
    
    id = floor(uv*grid);
    vec3 n = N13(id.x*35.2f+id.y*2376.1f);
    vec2 st = fract(uv*grid)-vec2(0.5f, 0.0f);
    
    float x = n.x-0.5f;
    
    float y = UV.y*20.0f;
    float wiggle = sin(y+sin(y));
    x += wiggle*(.5-abs(x))*(n.z-0.5f);
    x *= 0.7f;
    float ti = fract(t+n.z);
    y = (Saw(0.85f, ti)-0.5f)*0.9f+0.5f;
    vec2 p = vec2(x, y);
    
    float d = length((st-p)*a.yx);
    
    float mainDrop = S(0.4f, 0.0f, d);
    
    float r = sqrt(S(1.0f, y, st.y));
    float cd = abs(st.x-x);
    float trail = S(0.23f*r, 0.15f*r*r, cd);
    float trailFront = S(-0.02f, 0.02f, st.y-y);
    trail *= trailFront*r*r;
    
    y = UV.y;
    float trail2 = S(0.2f*r, 0.0f, cd);
    float droplets = max(0.0f, (sin(y*(1.0f-y)*120.0f)-st.y))*trail2*trailFront*n.z;
    y = fract(y*10.0f)+(st.y-0.5f);
    float dd = length(st-vec2(x, y));
    droplets = S(0.3f, 0.0f, dd);
    float m = mainDrop+droplets*r*trailFront;
    
    //m += st.x>a.y*.45 || st.y>a.x*.165 ? 1.2 : 0.;
    return vec2(m, trail);
}

float StaticDrops(vec2 uv, float t) {
    uv *= 40.0f;
    
    vec2 id = floor(uv);
    uv = fract(uv)-0.5f;
    vec3 n = N13(id.x*107.45f+id.y*3543.654f);
    vec2 p = (n.xy-0.5f)*0.7f;
    float d = length(uv-p);
    
    float fade = Saw(0.025f, fract(t+n.z));
    float c = S(0.3f, 0.0f, d)*fract(n.z*10.0f)*fade;
    return c;
}

vec2 Drops(vec2 uv, float t, float l0, float l1, float l2) {
    float s = StaticDrops(uv, t)*l0;
    vec2 m1 = DropLayer2(uv, t)*l1;
    vec2 m2 = DropLayer2(uv*1.85f, t)*l2;
    
    float c = s+m1.x+m2.x;
    c = S(0.3f, 1.0f, c);
    
    return vec2(c, max(m1.y*l0, m2.y*l1));
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
    
    vec2 uv = (fragCoordViewportCoordinates.xy-0.5f*resolution.xy) / resolution.y;
    vec2 UV = fragCoordViewportCoordinates.xy/resolution.xy;
    vec3 fullMouse = vec3(mouse.x, mouse.y, mouseDown);
    vec3 M = fullMouse / vec3(resolution.x, resolution.y, 1.0f);
    float T = time+M.x*2.0f;
    
#ifdef HAS_HEART
    T = mod(time, 102.0f);
    T = mix(T, M.x*102.0f, M.z>0.0f?1.0f:0.0f);
#endif
    
    
    float t = T*0.2f;
    
    float rainAmount = fullMouse.z > 0.0f ? M.y : sin(T*0.05f)*0.3f+0.7f;
    
    float maxBlur = mix(3.0f, 6.0f, rainAmount);
    float minBlur = 2.0f;
    
    float story = 0.0f;
    float heart = 0.0f;
    
    
#ifdef HAS_HEART
    story = S(0.0f, 70.0f, T);
    
    t = min(1.0f, T/70.0f);                        // remap drop time so it goes slower when it freezes
    t = 1.0f-t;
    t = (1.0f-t*t)*70.0f;
    
    float zoom= mix(0.3f, 1.2f, story);        // slowly zoom out
    uv *=zoom;
    minBlur = 4.0f+S(0.5f, 1.0f, story)*3.0f;        // more opaque glass towards the end
    maxBlur = 6.0f+S(0.5f, 1.0f, story)*1.5f;
    
    vec2 hv = uv-vec2(0.0f, -0.1f);                // build heart
    hv.x *= 0.5f;
    float s = S(110.0f, 70.0f, T);                // heart gets smaller and fades towards the end
    hv.y-=sqrt(abs(hv.x))*0.5f*s;
    heart = length(hv);
    heart = S(0.4f*s, 0.2f*s, heart)*s;
    rainAmount = heart;                        // the rain is where the heart is
    
    maxBlur-=heart;                            // inside the heart slighly less foggy
    uv *= 1.5f;                                // zoom out a bit more
    t *= 0.25f;
#else
    float zoom = -cos(T*0.2f);
    uv *= 0.7f+zoom*0.3f;
#endif
    
    UV = (UV-0.5f)*(0.9f+zoom*0.1f)+0.5f;
    
    float staticDrops = S(-0.5f, 1.0f, rainAmount)*2.0f;
    float layer1 = S(0.25f, 0.75f, rainAmount);
    float layer2 = S(0.0f, 0.5f, rainAmount);
    
    
    vec2 c = Drops(uv, t, staticDrops, layer1, layer2);
#ifdef CHEAP_NORMALS
    vec2 n = vec2(dFdx(c.x), dFdy(c.x));// cheap normals (3x cheaper, but 2 times shittier ;))
#else
    vec2 e = vec2(0.001f, 0.0f);
    float cx = Drops(uv+e, t, staticDrops, layer1, layer2).x;
    float cy = Drops(uv+e.yx, t, staticDrops, layer1, layer2).x;
    vec2 n = vec2(cx-c.x, cy-c.x);        // expensive normals
#endif
    
    
#ifdef HAS_HEART
    n *= 1.0f-S(60.0f, 85.0f, T);
    c.y *= 1.0f-S(80.0f, 100.0f, T)*0.8f;
#endif
    
    float focus = mix(maxBlur-c.y, minBlur, S(0.1f, 0.2f, c.x));
    vec3 col = textureLod(material.ambientMap, UV+n, focus).rgb;
    
#ifdef USE_POST_PROCESSING
    t = (T+3.0f)*0.5f;                                        // make time sync with first lightnoing
    float colFade = sin(t*0.2f)*0.5f+0.5f+story;
    col *= mix(vec3(1.0f), vec3(0.8f, 0.9f, 1.3f), colFade);    // subtle color shift
    float fade = S(0.0f, 10.0f, T);                            // fade in at the start
    float lightning = sin(t*sin(t*10.0f));                // lighting flicker
    lightning *= pow(max(0., sin(t+sin(t))), 10.0f);        // lightning flash
    col *= 1.0f+lightning*fade*mix(1.0f, 0.1f, story*story);    // composite lightning
    col *= 1.0f-dot(UV-=0.5f, UV);                            // vignette
    
#ifdef HAS_HEART
    col = mix(pow(col, vec3(1.2f)), col, heart);
    fade *= S(102.0f, 97.0f, T);
#endif
    
    col *= fade;                                        // composite start and end fade
#endif
    
    vec2 st = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (st.x < (  coverage  ) )
    {
        //col = vec3(heart);
        tc = vec4(col, 1.0f);
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
