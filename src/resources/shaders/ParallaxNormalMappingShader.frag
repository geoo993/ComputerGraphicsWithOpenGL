#version 400 core

// https://learnopengl.com/#!Advanced-Lighting/Parallax-Mapping

#define NUMBER_OF_POINT_LIGHTS 10

precision highp float;

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

// Structure holding light information:  its position, colors, direction etc...
struct BaseLight
{
    vec3 color;
    float intensity;
    float ambient;
    float diffuse;
    float specular;
};

struct Attenuation
{
    float constant;
    float linear;
    float exponent;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct PointLight
{
    BaseLight base;
    Attenuation attenuation;
    vec3 position;
    float range;
};

struct SpotLight
{
    PointLight pointLight;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

uniform DirectionalLight R_directionallight;
uniform PointLight R_pointlight[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight R_spotlight;
uniform bool bUseBlinn, bUseSmoothSpot;
uniform bool bUseDirectionalLight, bUsePointLight, bUseSpotlight;
uniform float heightScale;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec3 vWorldTangent;
    vec3 vWorldBiTangent;
    vec4 vEyePosition;
} fs_in;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, float scale)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0f / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0f;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * scale; 
    vec2 deltaTexCoords = P / numLayers;
    
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(material.displacementMap, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(material.displacementMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    
    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(material.displacementMap, prevTexCoords).r - currentLayerDepth + layerDepth;
    
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0f - weight);

    return finalTexCoords;
}

vec4 CalcLight(BaseLight base, vec3 direction, mat3 TBN, vec3 vertexPosition)
{
    vec3 viewPosition =  camera.position + camera.front;
    vec3 tangentViewPos         = TBN * viewPosition;
    vec3 tangentFragPos         = TBN * vertexPosition;
    
    vec3 directionToEye = normalize(tangentViewPos - tangentFragPos); // viewDirection
    vec2 texCoords = fs_in.vTexCoord;
    
    texCoords = ParallaxMapping(fs_in.vTexCoord,  directionToEye, heightScale);
    if(texCoords.x > 1.0f || texCoords.y > 1.0f || texCoords.x < 0.0f || texCoords.y < 0.0f) {
        discard;
    }
    
     // obtain normal from normal map in range [0,1]
     vec3 normalMap = texture(material.normalMap, fs_in.vTexCoord).rgb;
    
     // transform normal vector to range [-1,1]
     normalMap = normalize(normalMap * 2.0f - 1.0f);  // this normal is in tangent space
    
     vec4 lightColor = vec4(base.color, 1.0f);
     vec4 materialColor = material.color;
    
    // ambient
     vec4 ambient = base.ambient * (material.bUseTexture ? texture(material.ambientMap, fs_in.vTexCoord ) : materialColor);
    
     // diffuse
     //vec3 lightDirection = normalize(tangentLightPos - tangentFragPos);
     float diffuseFactor = max(dot(direction, normalMap), 0.0f);
     vec4 diffuse = base.diffuse * diffuseFactor * (material.bUseTexture ? texture( material.diffuseMap, fs_in.vTexCoord ) : materialColor);
    
     // specular
     vec3 reflectDirection = reflect(-direction, normalMap);    // specular reflection
     vec3 halfDirection = normalize(direction + directionToEye); // halfway vector
     float specularFactor = bUseBlinn
     ? pow(max(dot(normalMap, halfDirection), 0.0f), material.shininess)
     : pow(max(dot(directionToEye, reflectDirection), 0.0f), material.shininess);
     vec4 specular = base.specular * specularFactor * (material.bUseTexture ? texture( material.specularMap, fs_in.vTexCoord) : materialColor);
     
    return (ambient + diffuse + specular) * base.intensity
    * (material.bUseColor ? lightColor : vec4(1.0f));
}

vec4 CalcDirectionalLight(DirectionalLight directionalLight, mat3 TBN, vec3 vertexPosition)
{
    return CalcLight(directionalLight.base, normalize(-directionalLight.direction), TBN, vertexPosition);
}

vec4 CalcPointLight(PointLight pointLight, mat3 TBN, vec3 vertexPosition)
{
    vec3 tangentLightPos    = TBN * pointLight.position;
    vec3 tangentFragPos     = TBN * vertexPosition;
    vec3 lightDirection     = normalize(tangentLightPos - tangentFragPos);
    float distanceToPoint   = length(pointLight.position - vertexPosition);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    vec4 color = CalcLight(pointLight.base, lightDirection, TBN, vertexPosition);
    
    // attenuation
    float attenuation = 1.0f / (pointLight.attenuation.constant + pointLight.attenuation.linear * distanceToPoint +
                                pointLight.attenuation.exponent * (distanceToPoint * distanceToPoint));
    return color * attenuation;
}

vec4 CalcSpotLight(SpotLight spotLight, mat3 TBN, vec3 vertexPosition)
{
    vec3 lightDirection = normalize(spotLight.pointLight.position - vertexPosition);
    float theta = max(dot(lightDirection, normalize(-spotLight.direction)), 0.0f);
    vec4 color = vec4( 0.0f, 0.0f, 0.0f, 0.0f);
    
    if(theta > spotLight.cutOff)
    {
        float epsilon = spotLight.cutOff - spotLight.outerCutOff;
        float intensity = bUseSmoothSpot
        ? (1.0f - (1.0f - theta) / (1.0f - spotLight.cutOff))
        : clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);
        color = CalcPointLight(spotLight.pointLight, TBN, vertexPosition) * intensity;
    }
    return color;
}

//When rendering into the current framebuffer, whenever a fragment shader uses the layout location specifier the respective colorbuffer of framebuffor colors array, which is used to render the fragments to that color buffer.
layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 tangent = normalize(fs_in.vWorldTangent);
    vec3 bitangent = normalize(fs_in.vWorldBiTangent);
    vec3 worldPos = fs_in.vWorldPosition;
    mat3 TBN = transpose(mat3(tangent, bitangent, normal));
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Directional lighting
    if (bUseDirectionalLight){
        vec4 directionalLight = CalcDirectionalLight(R_directionallight, TBN, worldPos);
        result += directionalLight;
    }
    
    // Point lights
    if (bUsePointLight){
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
            vec4 pointL = CalcPointLight(R_pointlight[i], TBN, worldPos);
            result += pointL;
        }
    }
    
    // Spot light
    if (bUseSpotlight){
        vec4 spotL = CalcSpotLight(R_spotlight, TBN, worldPos);
        result += spotL;
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
    vAlbedoSpec.a = 1.0f;
}
