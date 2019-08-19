#version 400 core

// https://www.shadertoy.com/view/ldB3Dh
// https://www.shadertoy.com/view/4sSGD1
// To keep her scary noggin in the frame it uses
// the channel time to pan the video.
// Then move back towards the text.

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
uniform float channelTime = 1.0f;
uniform float width;   // width of the current render target
uniform float height;  // height of the current render target
uniform float coverage;        // between (0.0f and 1.0f)

mat2 RotateMat(float angle)
{
    float si = sin(angle);
    float co = cos(angle);
    return mat2(co, si, -si, co);
}


vec3 Colour(in float h)
{
    h = h * 4.0f;
    return clamp( abs(mod(h+vec3(0.0f,4.0f,2.0f),6.0f)-3.0f)-1.0f, 0.0f, 1.0f );
}


out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        // https://gamedev.stackexchange.com/questions/106674/shadertoy-getting-help-moving-to-glsl
        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml
        // https://stackoverflow.com/questions/26965787/how-to-get-accurate-fragment-screen-position-like-gl-fragcood-in-vertex-shader
        vec2 fragCoord = gl_FragCoord.xy;
        vec2 fragCoordViewportCoordinates = fragCoord * 0.5f + 0.5f;
        vec2 resolution = vec2(width, height);// width and height of the screen
        vec4 vTexColour = texture(material.ambientMap, fs_in.vTexCoord);
        
        // Rough panning...
        vec2 pixel = (fragCoordViewportCoordinates.xy - resolution.xy * 0.5f)/resolution.xy
        + vec2(0.0f, 0.1f -smoothstep(9.0f, 12.0f, channelTime) * 0.35f
               + smoothstep(18.0f, 20.0, channelTime) * 0.15f);
        
        vec3 col;
        float n;
        float inc = (smoothstep(17.35f, 18.5f, channelTime)-smoothstep(18.5f, 21.0f, channelTime)) * (channelTime-16.0f) * 0.1f;
        
        mat2 rotMat = RotateMat(inc);
        for (int i = 1; i < 50; i++)
        {
            pixel = pixel * rotMat;
            
            float depth = 40.0f + float(i) + smoothstep(18.0f, 21.0f, channelTime) * 65.0f;
            
            vec2 uv = pixel * depth / 210.0f;
            
            // Shifting the pan to the text near the end...
            
            // And shifts to the right again for the last line of text at 23:00!
            col = texture( material.ambientMap, fract(uv+vec2(0.5f + smoothstep(20.0f, 21.0f, channelTime) * 0.11f
                                                              + smoothstep(23.0f, 23.5f, channelTime) * 0.04f
                                                              , .7-smoothstep(20.0f, 21.0f, channelTime) * 0.2f))).rgb;
            col = mix(col, col * Colour((float(i)/50.0f+time)), smoothstep(18.5f, 21.5f, channelTime));
            
            if ((1.0f-(col.y*col.y)) < float(i+1) / 50.0f)
            {
                break;
            }
            
        }
        
        // Some contrast...
        col = min(col*col*1.5f, 1.0f);
        // Fade to red evil face...
        float gr = smoothstep(17.0f, 16.0f, channelTime) + smoothstep(18.5f, 21.0f, channelTime);
        float bl = smoothstep(17.0f, 15.0f, channelTime) + smoothstep(18.5f, 21.0f, channelTime);
        col = col * vec3(1.0f, gr, bl);
        // Cut off the messy end...
        col *= smoothstep(29.5f, 28.2f, channelTime);
        tc = vec4(col, 1.0f);
    } else if (uv.x >= ( coverage + 0.003f) ) {
        tc = texture(material.ambientMap, uv);
    } else {
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
}
