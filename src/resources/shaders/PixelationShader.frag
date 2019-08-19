#version 400 core

// http://www.geeks3d.com/20101029/shader-library-pixelation-post-processing-effect-glsl/

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

uniform float coverage;  // offset of the vertical red line, between (0 and 1)
uniform float pixel_width;  // width of a low resolution pixel = 15.0f
uniform float pixel_height;  // height of a low resolution pixel = 10.0f

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    /*   Pixelation Shader
     // This new shader shows how to pixelate the final rendering with a pixelation post processing GLSL shader. Pixelation effect lowers the definition of an image and increases the size of the individual image pixels.
    
     */
    
    vec2 uv = fs_in.vTexCoord.xy;
    
    vec4 tc = material.guiColor;
    if (uv.x < (coverage) )
    {
        float dx = pixel_width * (1.0f / width);
        float dy = pixel_height * (1.0f / height);
        vec2 coord = vec2(dx * floor(uv.x / dx),
                          dy * floor(uv.y / dy));
        //tc = texture(material.ambientMap, coord).rgb;
        tc = texture(material.ambientMap, coord);
    }
    else if ( uv.x >= (coverage + 0.003f )  )
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
