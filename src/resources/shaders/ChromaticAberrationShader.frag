#version 410 core

// http://www.geeks3d.com/20101008/shader-library-chromatic-aberration-demo-glsl/

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
    bool bUseTexture;
    bool bUseColor;
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
} fs_in;

uniform vec2 fresnelValues;
uniform vec3 IoR_Values;

vec3 refraction(vec3 i, vec3 n, float eta)
{
    float cosi = dot(-i, n);
    float cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
    vec3 t = eta * i + ((eta * cosi - sqrt(abs(cost2))) * n);
    return t * vec3(cost2 > 0.0f);
}

//
// fresnel approximation
// F(a) = F(0) + (1- cos(a))^5 * (1- F(0))
//
// Calculate fresnel term. You can approximate it 
// with 1.0-dot(normal, viewpos).    
//
float fast_fresnel(vec3 I, vec3 N, vec2 fresnel)
{
    float bias = fresnel.x;
    float power = fresnel.y;
    float scale = 1.0f - bias;
    return bias + pow(1.0f - dot(I, N), power) * scale;
}

float very_fast_fresnel(vec3 I, vec3 N)
{ 
    return 1.0f - dot(N, I); 
}

//When rendering into the current framebuffer, whenever a fragment shader uses the layout location specifier the respective colorbuffer of framebuffor colors array, which is used to render the fragments to that color buffer.
layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{
 
    vec3 I = normalize(fs_in.vWorldPosition - camera.position);
    vec3 N = normalize(fs_in.vWorldNormal);
    
    //------ Find the reflection
    //
    vec3 reflVec = reflect(I, N);
    vec3 reflectColor = texture(material.cubeMap, normalize(reflVec)).rgb;
    
    //------ Find the refraction
    //
    // Indices of refraction
    float Air = 1.0f;
    float Glass = 1.51714f;
    
    // Air to glass ratio of the indices of refraction (Eta)
    float Eta = Air / Glass;
    vec3 refracVec = refract(I, N, Eta);
    vec3 refractColor = texture(material.cubeMap, normalize(refracVec)).rgb;
    
    //vec3 refractColor;
    //refractColor.x = texture(material.cubeMap, refraction(I, N, IoR_Values.x)).x;
    //refractColor.y = texture(material.cubeMap, refraction(I, N, IoR_Values.y)).y;
    //refractColor.z = texture(material.cubeMap, refraction(I, N, IoR_Values.z)).z;
    
    vec3 baseColor = texture(material.diffuseMap, fs_in.vTexCoord.st).rgb;
    
    //------ Do a gloss map look up and compute the reflectivity.
    //
    vec3 glossColor = texture(material.glossinessMap, fs_in.vTexCoord.st).rgb;
    float reflectivity = (glossColor.r + glossColor.g + glossColor.b) / 3.0f;
    
    //------ Find the Fresnel term
    //
    float fresnelTerm = fast_fresnel(-I, N, fresnelValues);
    //float fresnelTerm = very_fast_fresnel(-I, N);
    
    //------ Write the final pixel.
    //
    vec3 color = mix(refractColor, reflectColor, fresnelTerm);
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (material.bUseTexture){
        result = vec4(mix(baseColor, color, reflectivity), 1.0f);
    }else {
        result = vec4(color, 1.0f);
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
    vOutputColour = vec4(envColor, result.w);
    
    
    
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
    vAlbedoSpec.rgb = material.bUseAO ? vec3(0.95f) : texture(material.glossinessMap, fs_in.vTexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    vAlbedoSpec.a = 1.0f;
}
