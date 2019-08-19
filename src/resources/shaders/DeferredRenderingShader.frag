#version 400 core

// https://learnopengl.com/#!Advanced-Lighting/Deferred-Shading
// https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/8.1.deferred_shading/8.1.deferred_shading.fs

#define NUMBER_OF_POINT_LIGHTS 10

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
    vec4 guiColor;
    float shininess;
    float uvTiling;
    bool bUseTexture;
    bool bUseColor;
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
uniform bool bUseBlinn, bUseSmoothSpot;
uniform bool bUseDirectionalLight, bUsePointLight, bUseSpotlight;
uniform float coverage;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vWorldNormal;
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
    
    vec4 ambientMap = texture(material.ambientMap, fs_in.vTexCoord);            // diffuserMap
    vec4 diffuseMap = texture(material.diffuseMap, fs_in.vTexCoord);            // diffuserMap
    float specularMap = texture(material.diffuseMap, fs_in.vTexCoord).a;        // specularMap
    
    vec4 lightColor = (material.bUseColor ? vec4(base.color, 1.0f) : vec4(1.0f));
    vec4 materialColor = material.color;
    vec4 ambient = base.ambient * (material.bUseTexture ? ambientMap : materialColor) * lightColor;
    vec4 diffuse = base.diffuse * diffuseFactor * (material.bUseTexture ? diffuseMap : materialColor) * lightColor;
    vec4 specular = base.specular * specularFactor * lightColor;
    if (material.bUseTexture) {
        specular *= specularMap;
    } else {
        specular *= materialColor;
    }
    
    return (ambient + diffuse + specular) * base.intensity;
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

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    vec2 uv = fs_in.vTexCoord.xy;
    vec4 tc = material.guiColor;
    
    if (uv.x <  coverage )
    {
        // retrieve data from gbuffer
        vec3 worldPos = texture(material.displacementMap, fs_in.vTexCoord).rgb;      // displacementMap
        vec3 normal = texture(material.normalMap, fs_in.vTexCoord).rgb;             // normalMap
        vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        
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
        
        tc = result;
        
    } else if (uv.x >= ( coverage + 0.003f) )
    {
        tc = texture(material.ambientMap, uv);
    } else {
        
        if ( coverage > ( 1.0f + 0.003f) ) {
            tc = texture(material.ambientMap, uv);
        }
    }
    
    vOutputColour = tc;
}
