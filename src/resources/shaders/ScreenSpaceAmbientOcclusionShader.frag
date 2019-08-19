#version 400 core

// https://learnopengl.com/#!Advanced-Lighting/SSAO
// http://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html

#define KERNEL_SIZE 64

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
    float uvTiling;
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

uniform mat4 projection;
uniform vec3 samples[KERNEL_SIZE];
uniform float radius = 0.5f;
uniform float bias = 0.025f;
uniform float width, height;
uniform float noiseSize = 4.0f;

out float vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    vec2 uv = fs_in.vTexCoord.xy;
    
    // tile noise texture over screen based on screen dimensions divided by noise size
    vec2 noiseScale = vec2(width / noiseSize, height / noiseSize);
    
    // retrieve data from gbuffer
    vec3 fragPos = texture(material.displacementMap, uv).xyz;                       // displacementMap
    vec3 normal = normalize(texture(material.normalMap, uv).rgb);                   // normalMap
    vec4 noise = texture(material.noiseMap, uv * noiseScale);                       // noise Map
    vec3 randomVec = normalize(noise.xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
     
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0f;
    for(int i = 0; i < KERNEL_SIZE; ++i)
    {
        // get sample position
        vec3 vSample = TBN * samples[i]; // from tangent to view-space
        vSample = fragPos + vSample * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(vSample, 1.0f);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5f + 0.5f; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(material.displacementMap, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(fragPos.z - sampleDepth));
        occlusion += ( (sampleDepth >= vSample.z + bias) ? 1.0f : 0.0f) * rangeCheck;   
    }
    occlusion = 1.0f - (occlusion / float(KERNEL_SIZE));
    
    vOutputColour = occlusion;
}
