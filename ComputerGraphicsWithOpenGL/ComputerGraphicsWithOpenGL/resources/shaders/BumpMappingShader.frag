#version 410 core

// http://www.geeks3d.com/20091019/shader-library-bump-mapping-shader-with-multiple-lights-glsl/
// http://www.ozone3d.net/tutorials/bump_mapping_p4.php
// http://fabiensanglard.net/bumpMapping/index.php

#define NUMBER_OF_POINT_LIGHTS 6

uniform struct Camera
{
    vec3 position;
    vec3 front;
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
    sampler2D reflectionMap;        // 10.  reflection map
    sampler2D depthMap;             // 11.  depth map
    sampler2D noiseMap;             // 12.  noise map
    sampler2D maskMap;              // 13.  mask map
    samplerCube cubeMap;            // 14.  sky box or environment mapping cube map
    vec3 color;
    float shininess;
} material;

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
uniform bool bUseTexture, bUseBlinn, bUseSmoothSpot;
uniform bool bUseDirectionalLight, bUsePointLight, bUseSpotlight;
uniform float uvTiling;

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

vec4 CalcLight(BaseLight base, vec3 direction, vec3 tangent, vec3 bitangent, vec3 normal, vec3 vertexPosition)
{
    
    vec3 v;
    v.x = dot(direction, tangent);
    v.y = dot(direction, bitangent);
    v.z = dot(direction, normal);
    vec3 lightVec = v;
    
    v.x = dot(-vertexPosition, tangent);
    v.y = dot(-vertexPosition, bitangent);
    v.z = dot(-vertexPosition, normal);
    vec3 viewVec = v;
    
    vec2 uv = fs_in.vTexCoord.st * uvTiling;
    
    // obtain normal from normal map in range [0,1]
    //vec3 ambientMap = texture(material.ambientMap, uv).rgb;
    vec3 normalMap = texture(material.normalMap, uv).rgb;
    vec3 diffuseMap = texture(material.diffuseMap, uv).rgb;
    //vec3 specularMap = texture(material.specularMap, uv).rgb;
    // ambient
    vec3 ambient = base.ambient * diffuseMap;
    
    // diffuse
    vec3 bump = normalize(normalMap * 2.0f - 1.0f);
    vec3 lVec = normalize(lightVec);
    vec3 vVec = normalize(viewVec);
    
    // diffuse
    float diffuseFactor = max(dot(lVec, bump), 0.0f);
    vec3 diffuse = base.diffuse * diffuseFactor * diffuseMap;
    
    // specular
    vec3 view =  camera.position + camera.front;
    vec3 directionToEye = normalize(view - vertexPosition); // viewDirection
    vec3 halfDirection = normalize(lVec + directionToEye); // halfway vector
    vec3 reflectDirection = reflect(-lVec, bump);    // specular reflection
    
    
    // diffuse
    //float diffuseFactor = max(dot(direction, normalMap), 0.0f);
    //vec3 diffuse = base.diffuse * diffuseFactor * diffuseMap;
    
    // specular
    //vec3 view =  camera.position + camera.front;
    //vec3 directionToEye = normalize(view - vertexPosition); // viewDirection
    //vec3 reflectDirection = reflect(-direction, normal);    // specular reflection
    //vec3 halfDirection = normalize(direction + directionToEye); // halfway vector
    
    float specularFactor = bUseBlinn
    ? pow(max(dot(bump, halfDirection), 0.0f), material.shininess)
    : pow(max(dot(vVec, reflectDirection), 0.0f), material.shininess);
    
    vec3 specular = vec3(base.specular) * specularFactor;
    return (bUseTexture ? vec4(ambient + diffuse + specular, 1.0f) : vec4(material.color, 1.0f)) * base.intensity * vec4(base.color, 1.0f);
}

vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 tangent, vec3 bitangent, vec3 normal, vec3 vertexPosition)
{
    return CalcLight(directionalLight.base, normalize(-directionalLight.direction), tangent, bitangent, normal, vertexPosition);
}

vec4 CalcPointLight(PointLight pointLight, vec3 tangent, vec3 bitangent, vec3 normal, vec3 vertexPosition)
{
    
    vec3 lightDirection = normalize(pointLight.position - vertexPosition);
    float distanceToPoint   = length(pointLight.position - vertexPosition);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    vec4 color = CalcLight(pointLight.base, lightDirection, tangent, bitangent, normal, vertexPosition);
    
    // attenuation
    float attenuation = 1.0f / (pointLight.attenuation.constant + pointLight.attenuation.linear * distanceToPoint +
                                pointLight.attenuation.exponent * (distanceToPoint * distanceToPoint));
    return color * attenuation;
}

vec4 CalcSpotLight(SpotLight spotLight, vec3 tangent, vec3 bitangent, vec3 normal, vec3 vertexPosition)
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
        color = CalcPointLight(spotLight.pointLight, tangent, bitangent, normal, vertexPosition) * intensity;
    }
    return color;
}

out vec4 vOutputColour;        // The output colour formely  gl_FragColor

void main()
{
    
  
    vec3 N = normalize(fs_in.vWorldNormal);
    vec3 T = normalize(fs_in.vWorldTangent);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec3 worldPos = fs_in.vWorldPosition;
    vec3 vVertex = fs_in.vEyePosition.xyz;
    
    // Directional lighting
    if (bUseDirectionalLight) {
        vec4 directionalLight = CalcDirectionalLight(R_directionallight, T, B, N, worldPos);
        result += directionalLight;
    }
    // Point lights
    if (bUsePointLight) {
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
            vec4 pointL = CalcPointLight(R_pointlight[i], T, B, N, worldPos);
            result += pointL;
        }
    }
    
    // Spot light
    if (bUseSpotlight) {
        vec4 spotL = CalcSpotLight(R_spotlight, T, B, N, worldPos);
        result += spotL;
    }
    
    vOutputColour = result;
}