#version 400 core

// http://www.geeks3d.com/20110219/shader-library-crosshatching-glsl-filter/

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


uniform int width;   // width of the current render target
uniform int height;  // height of the current render target

uniform float coverage;
uniform float hatch_y_offset = 5.0f;
uniform float lum_threshold_1 = 1.0f;
uniform float lum_threshold_2 =  0.7f;
uniform float lum_threshold_3 = 0.5f;
uniform float lum_threshold_4 = 0.3f;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    /*  Crosshatching 
         this is a simple cross-hatching pixel shader.
         What is the cross-hatching? From wikipedia:
     
         Hatching (hachure in French) is an artistic technique used to create tonal or shading effects by drawing (or painting or scribing) closely spaced parallel lines. (It is also used in monochromatic heraldic representations to indicate what the tincture of a “full-colour” emblazon would be.) When lines are placed at an angle to one another, it is called cross-hatching.
     
     
         About gl_FragCoord ->   http://www.txutxi.com/?p=182
         In GLSL, and more specificly in the fragment language, gl_FragCoord is an input variable that contains the window relative coordinate (x, y, z, 1/w) values for the fragment.
     */
    
    vec2 uv = fs_in.vTexCoord.xy;
    
    vec4 tc = material.guiColor;
    
    if (uv.x < (  coverage  )  )
    {
        float lum = length(  texture(material.ambientMap, uv).rgb  );
        tc = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        if (lum < lum_threshold_1)
        {
            if (mod(  gl_FragCoord.x + gl_FragCoord.y, 10.0f) == 0.0f)
                tc = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        
        if (lum < lum_threshold_2)
        {
            if (mod( gl_FragCoord.x - gl_FragCoord.y, 10.0f) == 0.0f)
                tc = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        
        if (lum < lum_threshold_3)
        {
            if (mod(  gl_FragCoord.x + gl_FragCoord.y - hatch_y_offset, 10.0f) == 0.0f)
                tc = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        
        if (lum < lum_threshold_4)
        {
            if (mod(gl_FragCoord.x - gl_FragCoord.y - hatch_y_offset, 10.0f) == 0.0f)
                tc = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    else if (uv.x   >=   (coverage + 0.003f)  )
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
