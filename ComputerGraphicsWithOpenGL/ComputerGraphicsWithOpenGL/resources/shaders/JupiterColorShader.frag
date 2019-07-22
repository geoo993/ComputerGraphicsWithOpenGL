#version 410 core

//https://www.clicktorelease.com/blog/vertex-displacement-noise-3d-webgl-glsl-three-js/
//https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
//https://thebookofshaders.com/11/
//https://www.clicktorelease.com/code/perlin/chrome.html

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
    bool bUseAO;
} material;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec3 vWorldTangent;
    vec4 vEyePosition;
} fs_in;

uniform vec2 mouse;
uniform float time;
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target

layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{
    // http://viclw17.github.io/2018/06/12/GLSL-Practice-With-Shadertoy/
    float speed = 0.1f;
    float scale = 0.002f;
    vec2 fragCoord = gl_FragCoord.xy * 0.5f + 0.5f;
    vec2 p = fragCoord * scale;
    for(int i=1; i<10; i++){
        p.x+=0.3f/float(i)*sin(float(i)*3.0f*p.y+time*speed)+mouse.x/1000.0f;
        p.y+=0.3f/float(i)*cos(float(i)*3.0f*p.x+time*speed)+mouse.y/1000.0f;
    }
    float r=cos(p.x+p.y+1.0f)*0.5f+0.5f;
    float g=sin(p.x+p.y+1.0f)*0.5f+0.5f;
    float b=(sin(p.x+p.y)+cos(p.x+p.y))*0.5f+0.5f;
    vec3 color = vec3(r,g,b);
    vOutputColour = vec4(color,1.0f);
    
    // Retrieve bright parts
    float brightness = dot(vOutputColour.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f) {
        vBrightColor = vec4(vOutputColour.rgb, 1.0f);
    } else {
        vBrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // store the fragment position vector in the first gbuffer texture
    vPosition = material.bUseAO ? fs_in.vEyePosition.xyz : fs_in.vWorldPosition;
    // also store the per-fragment normals into the gbuffer
    vNormal = normalize(fs_in.vWorldNormal);
    // and the diffuse per-fragment color
    vAlbedoSpec = material.bUseAO ? vec4(0.95f, 0.95f, 0.95f, 1.0f) : texture(material.noiseMap, fs_in.vTexCoord);
}

