#version 400 core

//http://john-chapman-graphics.blogspot.co.uk/2013/01/what-is-motion-blur-motion-pictures-are.html
// http://john-chapman-graphics.blogspot.com/2013/01/per-object-motion-blur.html

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
    sampler2D depthMap;             // 11.  depth map
    sampler2D noiseMap;             // 12.  noise map
    sampler2D maskMap;              // 13.  mask map
    samplerCube cubeMap;            // 14.  sky box or environment mapping cube map
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

/*
uniform sampler2D sampler0; // used to access the rendered image from pass 1
uniform sampler2D emptySampler0; // and empty sampler used to access the depth buffer
 */
uniform mat4 invMVP;
uniform mat4 prevMVP;
uniform float currentFPS;
uniform float targetFPS;
uniform float coverage;        // between (0.0f and 1.0f)


out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    /*
    // Unproject the point to 3D using the current modelview and projection matrix
    float p_depth = texture(emptySampler0, vTexCoord).r;
    vec2 p_ndc = vTexCoord.xy * 2.0f - 1.0f;
    vec4 current = invMVP * vec4(p_ndc, p_depth, 1.0f);
    current = current / current.w;
    
    // Project the point using the previous frame's modelview and projection matrix
    vec4 previous = prevMVP * current;
    previous.xyz /= previous.w; // normalised device coordinates
    previous.xy = previous.xy * 0.5f + vec2(0.5f); // texture coordinates
    
    //scale the blur according to frame rate
    float blurScale = float(currentFPS) / targetFPS;
    
    // Construct the blur vector (scaled for extra blur)
    vec2 blurVec = (previous.xy - vTexCoord) * blurScale;;
    
    // perform blur:
    vec4 result = texture(sampler0, vTexCoord);
    
    int nSamples = 10;
    for (int i = 1; i < nSamples; i++) {
        // get offset in range [-0.5, 0.5]:
        vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5f);
        
        // sample & add to result:
        result += texture(sampler0, vTexCoord + offset);
    }
    
    result /= float(nSamples);
    vOutputColour = result;
    */
    vOutputColour = vec4(1.0f);
}

