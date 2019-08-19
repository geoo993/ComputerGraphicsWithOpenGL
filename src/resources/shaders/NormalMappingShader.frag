#version 400 core

// http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
// https://learnopengl.com/#!Advanced-Lighting/Normal-Mapping
// http://www.thetenthplanet.de/archives/1180
// 

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
uniform bUseBlinn, bUseSmoothSpot;
uniform bool bUseDirectionalLight, bUsePointLight, bUseSpotlight;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
    vec3 vWorldTangent;
    vec4 vEyePosition;
} fs_in;


vec4 CalcLight(BaseLight base, vec3 direction, mat3 TBN, vec3 vertexPosition)
{
   
    vec3 viewPosition =  camera.position + camera.front;
    vec3 tangentViewPos         = TBN * viewPosition;
    vec3 tangentFragPos         = TBN * vertexPosition;
    
    // obtain normal from normal map in range [0,1]
    vec3 ambientMap = texture(material.ambientMap, fs_in.vTexCoord).rgb;
    vec3 normalMap = texture(material.normalMap, fs_in.vTexCoord).rgb;
    vec3 diffuseMap = texture(material.diffuseMap, fs_in.vTexCoord).rgb;
    vec3 specularMap = texture(material.specularMap, fs_in.vTexCoord).rgb;
    
    // transform normal vector to range [-1,1]
    normalMap = normalize(normalMap * 2.0f - 1.0f);  // this normal is in tangent space
    
    // ambient
    vec3 ambient = base.ambient * ambientMap;
    
    // diffuse
    float diffuseFactor = max(dot(direction, normalMap), 0.0f);
    vec3 diffuse = base.diffuse * diffuseFactor * diffuseMap;
    
    // specular
    vec3 directionToEye = normalize(tangentViewPos - tangentFragPos); // viewDirection
    vec3 reflectDirection = reflect(-direction, normalMap);    // specular reflection
    vec3 halfDirection = normalize(direction + directionToEye); // halfway vector
    float specularFactor = bUseBlinn
    ? pow(max(dot(normalMap, halfDirection), 0.0f), material.shininess)
    : pow(max(dot(directionToEye, reflectDirection), 0.0f), material.shininess);
    vec3 specular = vec3(base.specular) * specularFactor * specularMap;
    
    return (material.bUseTexture ? vec4(ambient + diffuse + specular, 1.0f) : material.color) * base.intensity
    * (material.bUseColor ? vec4(base.color, 1.0f) : vec4(1.0f));
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
  
    /*
         Tangent vectors (or TBN for Tangent Binormal Normal) are useful in many situations like bump/normal mapping and the TBN vectors are usually precomputed from texture coordinates and stored as a vertex attribute.
     
     
     */
    
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 tangent = normalize(fs_in.vWorldTangent);
    vec3 worldPos = fs_in.vWorldPosition;
    vec3 T = normalize(tangent - dot(tangent, normal) * normal);
    vec3 B = cross(normal, tangent);
    mat3 TBN = transpose(mat3(T, B, normal));
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
    vAlbedoSpec.rgb = material.bUseAO ? vec3(0.95f) :texture(material.diffuseMap, fs_in.vTexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    vAlbedoSpec.a = material.bUseAO ? 1.0f : texture(material.specularMap, fs_in.vTexCoord).r;
}
