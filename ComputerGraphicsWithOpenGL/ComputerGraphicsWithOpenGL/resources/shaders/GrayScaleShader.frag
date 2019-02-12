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
    sampler2D reflectionMap;        // 10.  reflection map
    samplerCube cubeMap;            // 11.  sky box or environment mapping cube map
    vec3 color;
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

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
 
    /*   https://learnopengl.com/#!Advanced-OpenGL/Framebuffers
         Grayscale
         Another interesting effect is to remove all colors from the scene except the white, gray and black colors effectively grayscaling the entire image. An easy way to do this is simply by taking all the color components and averaging their results:
     
     */
    
    vec4 result = texture(material.ambientMap, fs_in.vTexCoord);
    //float average = (result.r + result.g + result.b) / 3.0f;
    float average = 0.2126f * result.r + 0.7152f * result.g + 0.0722f * result.b;
    vOutputColour = vec4(average, average, average, 1.0f);
    

    
}
