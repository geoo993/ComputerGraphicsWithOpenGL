#version 400 core
// https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

uniform struct Shadow
{
    float znear;
    float zfar;
    float bias;
    bool bFromLightOrCamera;
    bool bShowDepth;
} shadow;

uniform vec3 lightPos;

in VS_OUT
{
    vec4 vWorldPosition;
} fs_in;

void main()
{
  
    float lightDistance = length(fs_in.vWorldPosition.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / shadow.zfar;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}
