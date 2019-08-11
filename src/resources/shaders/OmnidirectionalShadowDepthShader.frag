#version 400 core
#define NUMBER_OF_POINT_LIGHTS 10
// https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

uniform struct Shadow
{
    float znear;
    float zfar;
    float bias;
    bool bFromLightOrCamera;
    bool bShowDepth;
} shadow;

struct DirectionalLight
{
    vec3 position;
};

struct PointLight
{
    vec3 position;
};

struct SpotLight
{
    PointLight pointLight;
};

uniform DirectionalLight R_directionallight;
uniform PointLight R_pointlight[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight R_spotlight;
uniform bool bUseDirectionalLight, bUsePointLight, bUseSpotlight;

in VS_OUT
{
    vec4 vWorldPosition;
} fs_in;

void main()
{
  
    float lightDistance = 0.0f;
    
    if (bUseDirectionalLight){
        // Directional lighting
        lightDistance += length(fs_in.vWorldPosition.xyz - R_directionallight.position);
    }
    
    if (bUsePointLight){
        // Point lights
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++){
            lightDistance += length(fs_in.vWorldPosition.xyz - R_pointlight[i].position);
        }
    }
    
    if (bUseSpotlight){
        // Spot light
        lightDistance += length(fs_in.vWorldPosition.xyz - R_spotlight.pointLight.position);
    }
    
    // float lightDistance = length(fs_in.vWorldPosition.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / shadow.zfar;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}
