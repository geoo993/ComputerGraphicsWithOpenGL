#version 400 core

// https://www.shadertoy.com/view/4s2GRR
// http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/5/

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
uniform vec2 mouse;
uniform bool isSixScreens;
uniform bool isFourScreens;
uniform bool isTwoScreens;
uniform bool isOneScreen;
uniform float coverage;

#define EPSILON 0.000011

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    /*  About gl_FragCoord ->   http://www.txutxi.com/?p=182
         In GLSL, and more specificly in the fragment language, gl_FragCoord is an input variable that contains the window relative coordinate (x, y, z, 1/w) values for the fragment.
     
     */
   
    vec2 st = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (st.x < (  coverage  ) )
    {
        vec2 resolution = vec2(width, height);
        
        //normalized coords with some cheat
        vec2 p = isOneScreen ? ( gl_FragCoord.xy / (resolution.x * 2.0f) ) : (gl_FragCoord.xy / resolution.x);
        
        //screen proportion or screen ratio
        float prop = resolution.x / resolution.y;
        
        //center coords
        vec2 m = isOneScreen ? vec2(0.5f, 0.5f / prop) : vec2(1.0f, 1.0f / prop);
        
        //vector from center to current fragment
        vec2 d = p - m;
        // distance of pixel from center
        float r = sqrt( dot(d, d) );
        //amount of effect
        float power = ( 2.0f * 3.141592f / (2.0f * sqrt(dot(m, m))) ) * (mouse.x / width - 0.5f);
        
        //radius of 1:1 effect
        float bind;
        if (power > 0.0f){
            bind = sqrt(dot(m, m));//stick to corners
        }
        else {
            //stick to borders
            if (prop < 1.0f)
                bind = m.x;
            else
                bind = m.y;
        }
        
        //Weird formulas
        vec2 uv;
        if (power > 0.0f)  //fisheye
            uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
        else if (power < 0.0f)  //antifisheye
            uv = m + normalize(d) * atan(r * -power * 10.0f) * bind / atan(-power * bind * 10.0f);
        else
            uv = p;//no effect for power = 1.0
        
        //Second part of cheat
        //for round effect, not elliptical
        vec3 col = vec3(0.0f);
        
        if (isSixScreens){
            col = texture(material.ambientMap, vec2(uv.x * prop, uv.y * prop)).xyz;
        } else if (isFourScreens) {
            col = texture(material.ambientMap, vec2(uv.x , uv.y * prop)).xyz;
        } else if (isTwoScreens) {
            col = texture(material.ambientMap, vec2( uv.x , uv.y / prop )).xyz;
        } else if (isOneScreen) {
            //col = texture(material.ambientMap, vec2( uv.x / (prop * 1.45f), uv.y / (prop * 1.0f) )).xyz;
            col = texture(material.ambientMap, vec2(uv.x, uv.y * prop)).xyz;
        }
        
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
