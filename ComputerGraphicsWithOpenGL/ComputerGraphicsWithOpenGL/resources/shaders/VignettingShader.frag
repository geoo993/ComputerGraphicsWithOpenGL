#version 400 core

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

uniform bool bTint, bUseSepia;
//RADIUS of our vignette, where 0.5 results in a circle fitting the screen
uniform float radius = 0.3f; // [0 - 0.5]
//softness of our vignette, between 0.0 and 1.0 or between 0.0 and 0.5
uniform float softness = 0.25f;
uniform float coverage;        // between (0.0f and 1.0f)

//sepia colour, adjust to taste
const vec3 SEPIA = vec3(1.2f, 1.0f, 0.8f);

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    vec4 texColour = texture(material.ambientMap, uv);
    
    if (uv.x < (  coverage  ) )
    {
        vec2 vectorFromCentrePosition = uv - vec2(0.5f);
        
        //determine the vector length of the center position
        float len = length(vectorFromCentrePosition);
        
        //use smoothstep to create a smooth vignette
        float vignette = 1.0f - smoothstep(radius, 1.0f - softness, len);
        
        
        if (bTint) {
            ////
            //apply the vignette with 50% opacity
            texColour.rgb = mix(texColour.rgb, texColour.rgb * vignette, 0.5f);
            
            vec3 finalColor;
            //convert to grayscale using NTSC conversion weights
            float gray = dot(texColour.rgb, vec3(0.299f, 0.587f, 0.114f));
            vec3 grayColor = vec3(gray);
            if (bUseSepia == true) {
                //SEPIA
                //create our sepia tone from some constant value
                vec3 sepiaColor = grayColor * SEPIA;
                finalColor = sepiaColor;
            } else {
                //GRAYSCALE
                finalColor = grayColor;
            }
            
            
            //again we'll use mix so that the sepia effect is at 75%
            texColour.rgb = mix(texColour.rgb, finalColor, 0.75f);
            ////
        }
        
        texColour.rgb *= vignette;
        tc = texColour ;
    }
    else if ( uv.x  >=  (  coverage  +   0.003f) )
    {
        tc = texColour;
    }
    else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texColour;
        }
    }
    
    vOutputColour = tc;
    
}
