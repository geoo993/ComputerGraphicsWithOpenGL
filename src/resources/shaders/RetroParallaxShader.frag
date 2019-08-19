#version 400 core

// https://www.shadertoy.com/view/4sSGD1
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
uniform float pixelSize; // between 1.0f and 20.0f
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    

    vec4 tc = material.guiColor;
    
    if (fs_in.vTexCoord.x <  coverage )
    {
        // https://gamedev.stackexchange.com/questions/106674/shadertoy-getting-help-moving-to-glsl
        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml
        // https://stackoverflow.com/questions/26965787/how-to-get-accurate-fragment-screen-position-like-gl-fragcood-in-vertex-shader
        vec2 fragCoord = gl_FragCoord.xy;
        vec2 fragCoordViewportCoordinates = fragCoord * 0.5f + 0.5f;
        vec2 resolution = vec2(width, height);// width and height of the screen
        
        vec2 pixel = fragCoordViewportCoordinates.xy - resolution.xy * 0.5f;
        
        // pixellate
        pixel = floor(pixel/pixelSize);
        
        vec2 offset = vec2(time*3000.0f,pow(max(-sin(time*0.2f),0.0f),2.0f)*16000.0f)/pixelSize;
        //vec2 offset = vec2(3000.0f,pow(max(-sin(0.2f),0.0f),2.0f)*16000.0f)/pixelSize;
        
        vec3 col;
        for ( int i=0; i < 8; i++ )
        {
            // parallax position, whole pixels for retro feel
            float depth = 20.0f+float(i);
            vec2 uv = pixel + floor(offset/depth);
            
            uv /= resolution.y;
            uv *= depth/20.0f;
            uv *= 0.4f*pixelSize;
            
            col = texture( material.ambientMap, uv+0.5f ).rgb;
            
            if ( 1.0f-col.y < float(i+1)/8.0f )
            {
                col = mix( vec3(0.4f,0.6f,0.7f), col, exp2(-float(i)*0.1f) );
                break;
            }
        }
        tc = vec4(col,1.0f);
        
    } else if (fs_in.vTexCoord.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, fs_in.vTexCoord.xy);
    } else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, fs_in.vTexCoord.xy);
        }
    }
    
    vOutputColour = tc;
    
}
