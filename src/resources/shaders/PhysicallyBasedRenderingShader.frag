#version 400 core
// https://3dcoat.com/pbr/
// https://marmoset.co/posts/basic-theory-of-physically-based-rendering/
// https://marmoset.co/posts/physically-based-rendering-and-you-can-too/
// https://www.allegorithmic.com/system/files/software/download/build/PBR_Guide_Vol.1.pdf
// https://learnopengl.com/PBR/Theory
// http://renderwonk.com/publications/s2010-shading-course/
// https://blog.selfshadow.com/publications/s2017-shading-course/
// https://blog.selfshadow.com/publications/s2013-shading-course/hoffman/s2013_pbs_physics_math_notes.pdf
// https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// http://www.codinglabs.net/article_physically_based_rendering.aspx
// http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx
// https://academy.allegorithmic.com/courses/the-pbr-guide-part-1
// https://academy.allegorithmic.com/courses/the-pbr-guide-part-2
// https://seblagarde.wordpress.com/2011/08/17/feeding-a-physical-based-lighting-mode/
// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
// http://wiki.polycount.com/wiki/PBR
// https://3dtextures.me/tag/pbr/
// https://freepbr.com/
// http://dominium.maksw.com/articles/physically-based-rendering-pbr/pbr-part-one/

/*
 here are different types of substances in real world. They can be classified in three main group: Insulators, semi-conductors and conductors.
 In game we are only interesting by two of them: Insulators (Dielectric materials) and conductors (Metallic materials).
 Artists should understand to which category a material belong to. This will have influence on diffuse and specular value to assign to this material.
 
 
 Dielectric materials are the most common materials. Their optical properties rarely vary much over the visible spectrum:
 water, glass, skin, wood, hair, leather, plastic, stone, concrete, ruby, diamond…
 Metals. Their optical properties vary over the visible spectrum: iron, aluminium, copper, gold, cobalt,  nickel, silver…
 
 
 DIFFUSE COLOR
 
 The advantage of using a physically based shading model is that we can choose diffuse color value based on real world value.
 We used two kind of references for our game :
 
 1. Taking picture of real world with the method explained by Henry Labounta in its GDC 2011 talk “Art Direction Tools for Photo Real Games”.
 - Wait an overcast day to reduce directional lighting information.
 - Shot a raw photography (mean don’t use gamma space mode but linear mode) containing an X-rite passport Macbeth chart.
 - Import and calibrate photo using exposure and blacks with the help of the reference Macbeth chart value.
 - The resulting sRGB picture will be used as reference for artists.
 
 2. Taking physical albedo value.
 Disclaimer : All the numerical values provided are in linear space. And lighting processing inside the shader must be in linear space.
 
 Charcoal       0.04
 Fresh asphalt  0.04
 Worn asphalt   0.12
 Bare soil      0.17
 Green grass    0.25
 Desert sand    0.40
 New concrete   0.55
 Ocean Ice      0.5–0.7
 Fresh snow     0.80–0.90
 
 
 
 
 SPECULAR COLOR
 
 Dielectric transparent material (water, glass, plastic, gemstones…) indices of refraction are largely available on the web.
 But lot of usual materials like concrete, fabric, rock, wood… are not. And it is far more difficult to measure index of refraction of such materials.
 
 Good rules for specular color range of dielectric materials are:
 - No value under 0.02
 - Common gemstones 0.05 - 0.17
 - Common liquids 0.02 - 0.04
 - When not finding reference for a dielectric material, setting a value of 0.04 (around plastic)
 
 Except gemstones, any dielectric material we will use should be in the range 0.02 - 0.05.
 Here are some sample values:
 
 Quartz    0.045593921
 ice       0.017908907
 Water     0.020373188
 Alcohol   0.01995505
 Glass     0.04
 Milk      0.022181983
 Ruby      0.077271957
 Crystal   0.111111111
 Diamond   0.171968833
 Skin      0.028
 
the linear RGB specular color values can be the following:
 
                R            G           B
 Silver      0.971519    0.959915    0.915324
 Aluminium   0.913183    0.921494    0.924524
 Gold        1           0.765557    0.336057
 Copper      0.955008    0.637427    0.538163
 Chromium    0.549585    0.556114    0.554256
 Nickel      0.659777    0.608679    0.525649
 Titanium    0.541931    0.496791    0.449419
 Cobalt      0.662124    0.654864    0.633732
 Platinum    0.672411    0.637331    0.585456
 
 
 */

#define NUMBER_OF_POINT_LIGHTS 10
#define PI 3.14159265359

uniform struct Camera
{
    vec3 position;
    vec3 front;
    float znear;
    float zfar;
    bool isMoving;
    bool isOrthographic;
} camera;

// Structure holding PBR material information:  its albedo, metallic, roughness, etc...
uniform struct Material
{
    sampler2D ambientMap;           // 0.   ambient map (albedo map)
    sampler2D diffuseMap;           // 1.   diffuse map (metallic map)
    sampler2D specularMap;          // 2.   specular map (roughness map)
    sampler2D normalMap;            // 3.   normal map
    sampler2D albedoMap;            // 15.  albedo map
    sampler2D metallicMap;          // 16.  metalness map
    sampler2D roughnessMap;         // 17.  roughness map
    sampler2D aoMap;                // 7.   ambient oclusion map
    samplerCube cubeMap;            // 18.  sky box cube map
    samplerCube irradianceMap;      // 19.  sky box irradiance cube map
    samplerCube shadowMap;          // 10.  shadow cube map
    
    vec4 color;
    
    // for more info look at https://marmoset.co/posts/physically-based-rendering-and-you-can-too/
    float  albedo;           // Albedo is the base color input, commonly known as a diffuse map.
    // all values below are between 0 and 1
    float metallic;         // Define how reflective a surface is when viewing head on, while Fresnel defines how reflective a surface is at grazing angles.
    float roughness;        // Microsurface defines how rough or smooth the surface of a material is. between 0 and 1
    float fresnel;          // Fresnel is the percentage of light that a surface reflects at grazing angles.
    float ao;               // Ambient occlusion(AO) represents large scale occluded light and is generally baked from a 3d model.
    float shininess;
    float uvTiling;
    bool bUseAO;
    bool bUseTexture;
    bool bUseColor;
} material;

uniform struct Fog {
    float maxDist;
    float minDist;
    vec3 color;
    bool bUseFog;
} fog;

// Structure holding hrd light information
uniform struct HRDLight
{
    float exposure;
    float gamma;
    bool bHDR;
} hrdlight;

// Structure holding light information:  its position, colors, direction etc...
struct BaseLight
{
    vec3 color;
    float intensity;
    float ambient;
    float diffuse;
    float specular;
    float exposure;
    float gamma;
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
    float range; // This returns a radius between roughly 1.0 and 5.0 based on the light's maximum intensity.
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
uniform bool bUseIrradiance, bUseBlinn, bUseSmoothSpot;
uniform bool bUseDirectionalLight, bUsePointLight, bUseSpotlight;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vWorldPosition;
    vec3 vNormal;
    vec3 vWorldNormal;
    vec3 vWorldTangent;
    vec4 vEyePosition;
} fs_in;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anyways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap(vec3 position, vec2 uv)
{
    vec3 tangentNormal = texture(material.normalMap, uv).xyz * 2.0f - 1.0f;
    
    vec3 Q1  = dFdx(position);
    vec3 Q2  = dFdy(position);
    vec2 st1 = dFdx(uv);
    vec2 st2 = dFdy(uv);
    
    vec3 N   = normalize(fs_in.vNormal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    
    return normalize(TBN * tangentNormal);
}

// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
// Normal distribution function
float DistributionGGX(vec3 N, vec3 V, vec3 H, vec3 R, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = bUseBlinn
    ? pow(max(dot(N, H), 0.0f), material.shininess)
    : pow(max(dot(V, R), 0.0f), material.shininess);
    float NdotH2 = NdotH * NdotH;
    
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = float(PI) * denom * denom;
    
    return num / denom;
}

// Geometry function
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;
    
    float num   = NdotV;
    float denom = NdotV * (1.0f - k) + k;
    
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

// Fresnel equation
// F0 is known from surface reflection at zero incidence or how much the surface reflects if looking directly at the surface.
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

// ----------------------------------------------------------------------------
//Lights
vec3 CalcLight(BaseLight base, vec3 direction, vec3 normal, vec3 worldPos)
{
    vec2 uv = fs_in.vTexCoord.st * material.uvTiling;
    vec3 albedo     = material.bUseTexture ? (pow(texture(material.albedoMap, uv).rgb, vec3(2.2f)) * material.albedo) : (material.color.xyz * material.albedo);
    float metallic  = material.bUseTexture ? (texture(material.metallicMap, uv).r * material.metallic) : material.metallic;
    float roughness = material.bUseTexture ? (texture(material.roughnessMap, uv).r * material.roughness) : material.roughness;
    
    vec3 directionToEye = normalize(camera.position - worldPos); // viewDirection aka V
    vec3 reflectDirection = reflect(-directionToEye, normal);    // specular reflection aka R
    vec3 halfDirection = normalize(directionToEye + direction); // halfway vector
    
    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0         = vec3(0.04f); // Glass indices of refraction
    F0              = mix(F0, albedo, metallic);
    
    float diffuseFactor = max(dot(normal, direction), 0.0f);
    float diffuse = base.diffuse * diffuseFactor; // diffuse light
    
    // Cook-Torrance BRDF
    float NDF       = DistributionGGX(normal, directionToEye, halfDirection, reflectDirection, roughness);
    float G         = GeometrySmith(normal, directionToEye, direction, roughness);
    vec3 F          = fresnelSchlick(max(dot(halfDirection, directionToEye), 0.0f), F0); // fresnel value
    
    //  calculate the Cook-Torrance BRDF
    vec3 cookTorrance    = NDF * G * F;
    float specularFactor = 4.0f * max(dot(normal, directionToEye), 0.0f) * max(dot(normal, direction), 0.0f) + 0.001f;
    vec3 spec       = base.specular * (cookTorrance / specularFactor);
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0f) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0f - metallic;
    
    vec3 specular   = (kD * albedo / float(PI) + spec); // specular light
    vec3 lightColor = material.bUseColor ? base.color : vec3(1.0f);
    
    return specular * diffuse * base.intensity * lightColor;
}
vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 worldPos)
{
    return CalcLight(directionalLight.base, normalize(-directionalLight.direction), normal, worldPos);
}
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 worldPos)
{
    vec3 lightDirection = normalize(pointLight.position - worldPos);
    float distanceToPoint = length(pointLight.position - worldPos);
    
    vec3 color = CalcLight(pointLight.base, lightDirection, normal, worldPos);
    // attenuation
    float attenuation = 1.0f / (pointLight.attenuation.constant
                                + pointLight.attenuation.linear
                                * distanceToPoint
                                + pointLight.attenuation.exponent
                                * (distanceToPoint * distanceToPoint));
    return color * attenuation;
}
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 worldPos)
{
    vec3 lightDirection = normalize(spotLight.pointLight.position - worldPos);
    float theta = max(dot(lightDirection, normalize(-spotLight.direction)), 0.0f);
    vec3 color = vec3( 0.0f, 0.0f, 0.0f);
    
    if(theta > spotLight.cutOff)
    {
        float epsilon = spotLight.cutOff - spotLight.outerCutOff;
        float intensity = bUseSmoothSpot
        ? (1.0f - (1.0f - theta) / (1.0f - spotLight.cutOff))
        : clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);
        color = CalcPointLight(spotLight.pointLight, normal, worldPos) * intensity;
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
    
    vec3 worldPos   = fs_in.vWorldPosition;
    vec2 uv = fs_in.vTexCoord.st * material.uvTiling;
    vec3 normal     = material.bUseTexture ? getNormalFromMap(worldPos, uv) : normalize(fs_in.vNormal);       // albedo map
    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    
    vec3 albedo     = material.bUseTexture ? (pow(texture(material.albedoMap, uv).rgb, vec3(2.2f)) * material.albedo) : (material.color.xyz * material.albedo);
    float metallic  = material.bUseTexture ? (texture(material.metallicMap, uv).r * material.metallic) : material.metallic;
    float roughness = material.bUseTexture ? (texture(material.roughnessMap, uv).r * material.roughness) : material.roughness;
    float ao        = material.bUseTexture ? (texture(material.aoMap, uv).r * material.ao) : material.ao;
    
    vec3 directionToEye = normalize(camera.position - worldPos); // viewDirection aka V
    vec3 reflectDirection = reflect(-directionToEye, normal);    // specular reflection aka R
    
    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04f); // Glass indices of refraction
    F0 = mix(F0, albedo, metallic);
 
    if (bUseDirectionalLight){
        // Directional lighting
        vec3 directionalLight = CalcDirectionalLight(R_directionallight, normal, worldPos);
        color += directionalLight;
    }

    if (bUsePointLight){
        // Point lights
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
            vec3 pointL = CalcPointLight(R_pointlight[i], normal, worldPos);
            color += pointL;
        }
    }

    if (bUseSpotlight){
        // Spot light
        vec3 spotL = CalcSpotLight(R_spotlight, normal, worldPos);
        color += spotL;
    }
    
    vec3 ambient = vec3(1.0f);
    if (bUseIrradiance) {
        // ambient lighting (we now use IBL as the ambient term)
        vec3 F = fresnelSchlickRoughness(max(dot(normal, directionToEye), 0.0), F0, roughness);
        
        vec3 kS = F;
        vec3 kD = 1.0f - kS;
        kD *= 1.0f - metallic;
        
        vec3 irradiance         = texture(material.irradianceMap, normal).rgb;
        vec3 brdfDiffuse        = irradiance * albedo;
        
        // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
        const float MAX_REFLECTION_LOD = 4.0f;
        vec3 prefilteredColor   = textureLod(material.cubeMap, reflectDirection, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf               = texture(material.specularMap, vec2(max(dot(normal, directionToEye), 0.0f), roughness)).rg;
        vec3 brdfSpecular       = prefilteredColor * (F * brdf.x + brdf.y);
        
        ambient                 = (kD * brdfDiffuse + brdfSpecular) * ao;
    } else {
        ambient                 = vec3(0.03f) * albedo * ao;
    }
    color += ambient;
    
    // FOG
    if (fog.bUseFog) {
        /*
         In the above code, we used the absolute value of the z coordinate as the distance from the
         camera. This may cause the fog to look a bit unrealistic in certain situations. To compute a
         more precise distance, we could replace the line:
            float dist = abs( Position.z );
         with the following.
            float dist = length( Position.xyz );
         */
        //float dist = abs( fs_in.vEyePosition.z );
        float dist = length( fs_in.vEyePosition.xyz );
        float fogFactor = (fog.maxDist - dist) / (fog.maxDist - fog.minDist);
        fogFactor = clamp( fogFactor, 0.0f, 1.0f );

        color += mix( fog.color, color, fogFactor );
    }

    /*
     Here we tone map the HDR color using the Reinhard operator, preserving the high dynamic range of possibly highly
     varying irradiance after which we gamma correct the color.
     We don't have a separate framebuffer or post-processing stage so we can directly apply both the tone mapping step
     and gamma correction step directly at the end of the forward fragment shader.
     
     Taking both linear color space and high dynamic range into account is incredibly important in a PBR pipeline.
     Without these it's impossible to properly capture the high and low details of varying light intensities and
     your calculations end up incorrect and thus visually unpleasing.
     */
    
    
    // HDR
    if(hrdlight.bHDR)
    {
        // tone mapping with exposure
        color = vec3(1.0f) - exp(-color * hrdlight.exposure);
        // also gamma correct while we're at it
        color = pow(color, vec3(1.0f / hrdlight.gamma));
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
    vOutputColour = vec4(color, 1.0f);
    
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
    vAlbedoSpec.rgb = material.bUseAO ? vec3(0.95f) : texture(material.albedoMap, uv).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    vAlbedoSpec.a = material.bUseAO ? 1.0f : texture(material.aoMap, uv).r;
    
}
