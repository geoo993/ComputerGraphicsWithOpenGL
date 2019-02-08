#version 400 core

// http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
// https://learnopengl.com/#!Advanced-Lighting/Normal-Mapping
// http://www.thetenthplanet.de/archives/1180
// 

#define NUMBER_OF_POINT_LIGHTS 6

precision highp float;

uniform struct Camera
{
    vec3 position;
    vec3 front;
} camera;

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
    
    vec3 color;
    float shininess;
} material;

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
uniform bool bUseTexture, bUseBlinn, bUseSmoothSpot;
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


vec4 CalcLight(BaseLight base, vec3 position, vec3 normal, vec3 tangent, vec3 vertexPosition)
{
    vec3 T = normalize(tangent - dot(tangent, normal) * normal);
    vec3 B = cross(normal, tangent);
    mat3 TBN = transpose(mat3(T, B, normal));
    vec3 viewPosition =  camera.position + camera.front;
    vec3 tangentLightPos        = TBN * position;
    vec3 tangentViewPos         = TBN * viewPosition;
    vec3 tangentFragPos         = TBN * vertexPosition;
    
    
    // obtain normal from normal map in range [0,1]
    //vec3 ambientMap = texture(material.ambientMap, fs_in.vTexCoord).rgb;
    vec3 normalMap = texture(material.normalMap, fs_in.vTexCoord).rgb;
    vec3 diffuseMap = texture(material.diffuseMap, fs_in.vTexCoord).rgb;
    //vec3 specularMap = texture(material.specularMap, fs_in.vTexCoord).rgb;
    
    // transform normal vector to range [-1,1]
    normalMap = normalize(normalMap * 2.0f - 1.0f);  // this normal is in tangent space
    // ambient
    vec3 ambient = base.ambient * diffuseMap;
    
    // diffuse
    vec3 lightDirection = normalize(tangentLightPos - tangentFragPos);
    float diffuseFactor = max(dot(lightDirection, normalMap), 0.0f);
    vec3 diffuse = base.diffuse * diffuseFactor * diffuseMap;
    
    // specular
    vec3 directionToEye = normalize(tangentViewPos - tangentFragPos); // viewDirection
    vec3 reflectDirection = reflect(-lightDirection, normalMap);    // specular reflection
    vec3 halfDirection = normalize(lightDirection + directionToEye); // halfway vector
    float specularFactor = bUseBlinn
    ? pow(max(dot(normalMap, halfDirection), 0.0f), material.shininess)
    : pow(max(dot(directionToEye, reflectDirection), 0.0f), material.shininess);
    vec3 specular = vec3(base.specular) * specularFactor;
    
    return (bUseTexture ? vec4(ambient + diffuse + specular, 1.0f) : vec4(material.color, 1.0f)) * base.intensity * vec4(base.color, 1.0f);
}

vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 tangent, vec3 vertexPosition)
{
    return CalcLight(directionalLight.base, vec3(1.0f), normal, tangent, vertexPosition);
}

vec4 CalcPointLight(PointLight pointLight, vec3 normal, vec3 tangent, vec3 vertexPosition)
{
    vec3 lightDirection = normalize(pointLight.position - vertexPosition);
    float distanceToPoint = length(pointLight.position - vertexPosition);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    vec4 color = CalcLight(pointLight.base, pointLight.position, normal, tangent, vertexPosition);
    
    // attenuation
    float attenuation = 1.0f / (pointLight.attenuation.constant + pointLight.attenuation.linear * distanceToPoint +
                                pointLight.attenuation.exponent * (distanceToPoint * distanceToPoint));
    return color * attenuation;
}

vec4 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 tangent, vec3 vertexPosition)
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
        color = CalcPointLight(spotLight.pointLight, normal, tangent, vertexPosition) * intensity;
    }
    return color;
}

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
  
    /*
         Tangent vectors (or TBN for Tangent Binormal Normal) are useful in many situations like bump/normal mapping and the TBN vectors are usually precomputed from texture coordinates and stored as a vertex attribute.
     
     
     */
    
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 tangent = normalize(fs_in.vWorldTangent);
    vec3 worldPos = fs_in.vWorldPosition;
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Directional lighting
    if (bUseDirectionalLight){
        vec4 directionalLight = CalcDirectionalLight(R_directionallight, normal, tangent, worldPos);
        result += directionalLight;
    }
    
    // Point lights
    if (bUsePointLight){
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
            vec4 pointL = CalcPointLight(R_pointlight[i], normal, tangent, worldPos);
            result += pointL;
        }
    }
    
    // Spot light
    if (bUseSpotlight){
        vec4 spotL = CalcSpotLight(R_spotlight, normal, tangent, worldPos);
        result += spotL;
    }
    
    vOutputColour = result;
}
