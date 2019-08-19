#version 400 core
//https://www.shadertoy.com/view/ldl3WM
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

uniform vec2 mouse;
uniform float time;
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

float eps = 0.007f;

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    vec2 uv = fs_in.vTexCoord.xy;// / resolution.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        vec3 t   = texture(material.ambientMap, uv).rgb;
        vec3 t00 = texture(material.ambientMap, uv+vec2(-eps,-eps)).rgb;
        vec3 t10 = texture(material.ambientMap, uv+vec2( eps,-eps)).rgb;
        vec3 t01 = texture(material.ambientMap, uv+vec2(-eps, eps)).rgb;
        vec3 t11 = texture(material.ambientMap, uv+vec2( eps, eps)).rgb;
        vec3 tm = (t00+t01+t10+t11)/4.0f;
        vec3 v=t; vec3 c;

        //t = .5+.5*sin(vec4(100.,76.43,23.75,1.)*t);
        t = t-tm;
        //t = 1.-t;
        t = t*t*t;
        //t = 1.-t;
        v=t;
        v = 10000.0f*t;

        float g = (tm.x-0.3f)*5.0f;
        //g = (g-.5); g = g*g*g/2.-.5;
        vec3 col0 = vec3(0.0f,0.0f,0.0f);
        vec3 col1 = vec3(0.2f,0.5f,1.0f);
        vec3 col2 = vec3(1.0f,0.8f,0.7f);
        vec3 col3 = vec3(1.0f,1.0f,1.0f);
        if      (g > 2.0f) c = mix(col2,col3,g-2.0f);
        else if (g > 1.0f) c = mix(col1,col2,g-1.0f);
        else             c = mix(col0,col1,g);

        c = clamp(c,0.0f,1.0f);
        v = clamp(v,0.0f,1.0f);
        v = c*(1.0f-v);
        //v = c-1.5*(1.-v); v = 1.-v;
        v = clamp(v,0.0f,1.0f);
        if (v==col0) v=col3;

        tc = vec4(v,1.0f);
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
