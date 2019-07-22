#version 400 core
// http://sunandblackcat.com/tipFullView.php?l=eng&topicid=15
// http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/toon-shader-version-ii/
// http://prideout.net/blog/?p=22

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

float toonIntensity;

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
    const float A = 0.1f;
    const float B = 0.3f;
    const float C = 0.6f;
    const float D = 1.0f;
    
    float diffuseFactor = max(dot(normal, direction), 0.0f);
  
    vec3 viewPosition =  camera.position + camera.front;
    vec3 directionToEye = normalize(viewPosition - vertexPosition); // viewDirection
    vec3 reflectDirection = reflect(-direction, normal);    // specular reflection
    vec3 halfDirection = normalize(direction + directionToEye); // halfway vector
    float specularFactor = bUseBlinn
    ? pow(max(dot(normal, halfDirection), 0.0f), material.shininess)
    : pow(max(dot(directionToEye, reflectDirection), 0.0f), material.shininess);
    
    // toon black edges
    vec4 lightColor = vec4(base.color, 1.0f);
    vec4 materialColor = material.color;
    float edgeMask = (dot(directionToEye, normal) < 0.4f) ? 0.0f : 1.0f;
    vec4 toonColor = edgeMask * lightColor;
    
    // toon intensity
    toonIntensity = diffuseFactor + specularFactor;
    
    vec4 ambient = base.ambient * (material.bUseTexture ? texture( material.ambientMap, fs_in.vTexCoord ) : materialColor);
    vec4 diffuse = base.diffuse * diffuseFactor * (material.bUseTexture ? texture( material.diffuseMap, fs_in.vTexCoord ) : materialColor);
    vec4 specular = base.specular * specularFactor * (material.bUseTexture ? texture( material.specularMap, fs_in.vTexCoord ) : materialColor);
    return (ambient + diffuse + specular + toonColor) * base.intensity * (material.bUseColor ? lightColor : vec4(1.0f));
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
layout (location = 0) out vec4 vOutputColour;   // The output colour formely  gl_FragColor
layout (location = 1) out vec4 vBrightColor;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormal;
layout (location = 4) out vec4 vAlbedoSpec;

void main()
{
    vec3 normal = normalize(fs_in.vWorldNormal);
    vec3 worldPos = fs_in.vWorldPosition;
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
    
    const float A = 0.1f;
    const float B = 0.3f;
    const float C = 0.6f;
    const float D = 1.0f;
    vec4 color = vec4(1.0f);
    
    if (toonIntensity < A) color = result * 0.001f;
    else if (toonIntensity < B) color = result * B;
    else if (toonIntensity < C) color = result * C;
    else color = result * D;
    
    // FOG
    vec3 fogColor = color.xyz;
    if (fog.bUseFog) {
        //float dist = abs( fs_in.vEyePosition.z );
        float dist = length( fs_in.vEyePosition.xyz );
        float fogFactor = (fog.maxDist - dist) / (fog.maxDist - fog.minDist);
        fogFactor = clamp( fogFactor, 0.0f, 1.0f );
        
        fogColor += mix( fog.color, fogColor, fogFactor );
    }
    color = vec4(fogColor, color.w);
    
    
    // HDR
    vec3 hdrColor = color.xyz;
    if(hrdlight.bHDR)
    {
        // tone mapping with exposure
        hdrColor = vec3(1.0f) - exp(-hdrColor * hrdlight.exposure);
        // also gamma correct while we're at it
        hdrColor = pow(hdrColor, vec3(1.0f / hrdlight.gamma));
    }
    /*
     else {
         // reinhard tone mapping
         color = color / (color + vec3(1.0f));
         /// gamma correct
         //color = pow(color, vec3(1.0f/2.2f));
         color = pow(color, vec3(1.0f / hrdlight.gamma));
         }
     */
    color = vec4(hdrColor, color.w);
    
    
    vOutputColour = color;

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
