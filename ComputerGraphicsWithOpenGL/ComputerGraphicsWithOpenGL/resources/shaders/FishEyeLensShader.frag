#version 400 core

// http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/6/

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
uniform float lensSize = 0.4f;
uniform vec3 lensOutlineColor; 
uniform float coverage;

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
        
        vec2 p = gl_FragCoord.xy / (resolution.xy * 2.0f);
        
        //vec2 movement = vec2(mouse.x, mouse.y );
        vec2 movement = vec2(mouse.x, (-mouse.y) + (height) );
        vec2 m = (movement) / resolution.xy;
        
        vec2 d = p - m;
        float r = sqrt(dot(d, d)); // distance of pixel from mouse
        
        vec2 uv;
        vec3 col = vec3(0.0f);
        if (r > lensSize + 0.005f)
        {
            uv = p;
            col = texture(material.ambientMap, vec2(uv.x, uv.y)).xyz;
        }
        else if (r < lensSize - 0.005f)
        {
            // Thanks to Paul Bourke for these formulas; see
            // http://paulbourke.net/miscellaneous/lenscorrection/
            // and .../lenscorrection/lens.c
            // Choose one formula to uncomment:
            // SQUAREXY:
            // uv = m + vec2(d.x * abs(d.x), d.y * abs(d.y));
            // SQUARER:
            uv = m + d * r; // a.k.a. m + normalize(d) * r * r
            // SINER:
            // uv = m + normalize(d) * sin(r * 3.14159 * 0.5);
            // ASINR:
            // uv = m + normalize(d) * asin(r) / (3.14159 * 0.5);
            col = texture(material.ambientMap, vec2(uv.x, uv.y)).xyz;
            
        }else {
            col = lensOutlineColor;
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
