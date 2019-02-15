#version 410 core
// https://github.com/BennyQBD/3DGameEngineCpp_60/tree/master/3DEngineCpp/res/shaders
// https://github.com/BennyQBD/3DGameEngineCpp_60/blob/master/3DEngineCpp/res/shaders/lighting.glh
// https://learnopengl.com/Lighting/Light-casters
// https://learnopengl.com/Lighting/Multiple-lights
// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting

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

vec4 CalcLight(BaseLight base, vec3 direction, vec3 normal, vec3 vertexPosition)
{
    float diffuseFactor = max(dot(normal, direction), 0.0f);
    
    vec3 view =  camera.position + camera.front;
    vec3 directionToEye = normalize(view - vertexPosition); // viewDirection
    vec3 reflectDirection = reflect(-direction, normal);    // specular reflection
    vec3 halfDirection = normalize(direction + directionToEye); // halfway vector
    float specularFactor = bUseBlinn
    ? pow(max(dot(normal, halfDirection), 0.0f), material.shininess)
    : pow(max(dot(directionToEye, reflectDirection), 0.0f), material.shininess);
    
    vec4 lightColor = vec4(base.color, 1.0f);
    vec4 materialColor = vec4(material.color, 1.0f);
    vec4 ambient = base.ambient * (bUseTexture ? texture( material.diffuseMap, fs_in.vTexCoord ) : materialColor);
    vec4 diffuse = base.diffuse * diffuseFactor * (bUseTexture ? texture( material.diffuseMap, fs_in.vTexCoord ) : materialColor);
    vec4 specular = base.specular * specularFactor * (bUseTexture ? texture( material.specularMap, fs_in.vTexCoord ) : materialColor);
    return (ambient + diffuse + specular) * base.intensity * lightColor;
}

vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 vertexPosition)
{
    return CalcLight(directionalLight.base, normalize(-directionalLight.direction), normal, vertexPosition);
}

vec4 CalcPointLight(PointLight pointLight, vec3 normal, vec3 vertexPosition)
{
    vec3 lightDirection = normalize(pointLight.position - vertexPosition);
    float distanceToPoint = length(pointLight.position - vertexPosition);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    vec4 color = CalcLight(pointLight.base, lightDirection, normal, vertexPosition);
 
    // attenuation
    
    float attenuation = 1.0f / (pointLight.attenuation.constant + pointLight.attenuation.linear * distanceToPoint +
                                pointLight.attenuation.exponent * (distanceToPoint * distanceToPoint));
    return color * attenuation;
    

    //float attenuation = (pointLight.attenuation.constant +
    //                     pointLight.attenuation.linear * distanceToPoint +
    //                     pointLight.attenuation.exponent * (distanceToPoint * distanceToPoint) + 0.0001f);
    //return color / attenuation;
}


vec4 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 vertexPosition)
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
        color = CalcPointLight(spotLight.pointLight, normal, vertexPosition) * intensity;
    }
    return color;
}

//When rendering into the current framebuffer, whenever a fragment shader uses the layout location specifier the respective colorbuffer of framebuffor colors array, which is used to render the fragments to that color buffer.
layout (location = 0) out vec4 vOutputColour; // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;

void main() {
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 worldPos = fs_in.vWorldPosition;
    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if (bUseDirectionalLight){
        // Directional lighting
        vec4 directionalLight = CalcDirectionalLight(R_directionallight, normal, worldPos);
        result += directionalLight;
    }
    
    if (bUsePointLight){
        // Point lights
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
            vec4 pointL = CalcPointLight(R_pointlight[i], normal, worldPos);
            result += pointL;
        }
    }
    
    if (bUseSpotlight){
        // Spot light
        vec4 spotL = CalcSpotLight(R_spotlight, normal, worldPos);
        result += spotL;
    }
    
    vOutputColour = result;
    
    // Retrieve bright parts
    float brightness = dot(vOutputColour.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
        vBrightColor = vec4(vOutputColour.rgb, 1.0f);
    else
        vBrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
}
