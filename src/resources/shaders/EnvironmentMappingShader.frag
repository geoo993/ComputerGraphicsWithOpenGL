#version 400 core

//https://learnopengl.com/#!Advanced-OpenGL/Cubemaps

uniform struct Camera
{
    vec3 position;
    vec3 front;
    float znear;
    float zfar;
    bool isMoving;
    bool isOrthographic;
} camera;

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
    bool bUseAO;
} material;

uniform struct HRDLight
{
    float exposure;
    float gamma;
    bool bHDR;
} hrdlight;

uniform struct Fog {
    float maxDist;
    float minDist;
    vec3 color;
    bool bUseFog;
} fog;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec4 vEyePosition;
    mat4 vInverseViewMatrix;
} fs_in;


vec3 CalcReflection(vec3 normal, vec3 vertexPosition)
{
    vec3 view =  camera.position + camera.front;
    vec3 directionToEye = normalize(vertexPosition - view); // viewDirection
    vec3 reflectDirection = reflect(directionToEye, normal);    // specular reflection
    return reflectDirection;
}

vec3 CalcRefraction(float ratio, vec3 normal, vec3 vertexPosition)
{
    vec3 view =  camera.position + camera.front;
    vec3 directionToEye = normalize(vertexPosition - view); // viewDirection
    vec3 refractionDirection = refract(directionToEye, normal, ratio);
    
    return refractionDirection;
}

uniform bool bRefraction;

layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{

    
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 worldPos = fs_in.vWorldPosition;
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    /*
     Reflection
     Reflection is the property that an object (or part of an object) reflects its surrounding environment e.g. the object's colors are more or less equal to its environment based on the angle of the viewer. A mirror for example is a reflective object: it reflects its surroundings based on the viewer's angle.
    
     To implement reflection, one must determine how the viewing vector from the
     camera reflects off the surface, using the reflect function in GLSL.
     Since the reflected vector must be in world coordinates, there are two options:
     ◦ Perform the reflection in world coordinates.
     This is the approach taken In the OpenGL Shading Language Cookbook
     It requires knowledge of the model matrix (independent of the view matrix)
     
     ◦ Perform the reflection in eye coordinates, then rotate the reflected point to
     world coordinates. It requires knowledge of the inverse view matrix.
     
     */
    
    // Reflect environment
    if (!bRefraction) {
        vec3 reflec = CalcReflection(normal, worldPos);
        result += vec4(texture(material.cubeMap, reflec).rgb, 1.0f);
    }
    
    
    /*
     Refraction
         Another form of environment mapping is called refraction and is similar to reflection. Refraction is the change in direction of light due to the change of the material the light flows through. Refraction is what we commonly see with water-like surfaces where the light doesn't enter straight through, but bends a little. It's like looking at your arm when it's halfway in the water.
     
         Refraction can easily be implemented by using GLSL's built-in refract function that expects a normal vector, a view direction and a ratio between both materials' refractive indices.
     
         The refractive index determines the amount light distorts/bends of a material where each material has its own refractive index. A list of the most common refractive indices are given in the following table:
     
         Material    Refractive index
         Air            1.00
         Water          1.33
         Ice            1.309
         Glass          1.52
         Diamond        2.42
         
         We use these refractive indices to calculate the ratio between both materials the light passes through. In our case, the light/view ray goes from air to glass (if we assume the container is made of glass) so the ratio becomes 1.00 / 1.52 = 0.658.
     
     */
    // glass like object refraction, which feels like a lens zoom
    
    if (bRefraction) {
        //float air = 1.0f;
        float glass = 1.52f;
        float ratio = 1.0f / glass;
        vec3 refrac = CalcRefraction(ratio, normal, worldPos);
        result += vec4(texture(material.cubeMap, refrac).rgb, 1.0f);
    }
    
    // FOG
    vec3 fogColor = result.xyz;
    if (fog.bUseFog) {
        //float dist = abs( fs_in.vEyePosition.z );
        float dist = length( fs_in.vEyePosition.xyz );
        float fogFactor = (fog.maxDist - dist) / (fog.maxDist - fog.minDist);
        fogFactor = clamp( fogFactor, 0.0f, 1.0f );
        
        fogColor += mix( fog.color, fogColor, fogFactor );
    }
    result = vec4(fogColor, result.w);
    
    // HDR
    vec3 envColor = result.xyz;
    if(hrdlight.bHDR)
    {
        // tone mapping with exposure
        envColor = vec3(1.0f) - exp(-envColor * hrdlight.exposure);
        // also gamma correct while we're at it
        envColor = pow(envColor, vec3(1.0f / hrdlight.gamma));
    }
//    else {
//        envColor = envColor / (envColor + vec3(1.0f));
//        envColor = pow(envColor, vec3(1.0f / hrdlight.gamma));
//    }
    result = vec4(envColor, result.w);
    
    
    vOutputColour = result;
    
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
    vAlbedoSpec.rgb = material.bUseAO ? vec3(0.95f) : texture(material.diffuseMap, fs_in.vTexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    vAlbedoSpec.a = material.bUseAO ? 1.0f : texture(material.specularMap, fs_in.vTexCoord).r;
}
