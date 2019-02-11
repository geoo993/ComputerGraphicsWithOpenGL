#version 400 core

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec4 outAlbedoSpec;

in vec2 vTexCoord;
in vec3 vWorldPosition;
in vec3 vLocalNormal;

uniform sampler2D sampler0;
uniform sampler2DMS screenTextureMS; 

uniform int width;   // width of the current render target
uniform int height;  // height of the current render target
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
  
    vec4 textColor = texture(sampler0, vTexCoord);
    //vec4 colorSample = texelFetch(screenTextureMS, vTexCoord, 3);  // 4th subsample
    vOutputColour = textColor;
   

    /*
    // Dark Effect
    float dx = 1.0f / float(width);
    float dy = 1.0f / float(height);
    vec4 thisTexel = texture(sampler0, vTexCoord);
    vec4 leftTexel = texture(sampler0, vTexCoord + vec2(-dx, 0.0));
    vec4 rightTexel = texture(sampler0, vTexCoord + vec2(dx, 0.0));
    vec4 topTexel = texture(sampler0, vTexCoord + vec2(0.0, dy));
    vec4 bottomTexel = texture(sampler0, vTexCoord + vec2(0.0, -dy));
    vOutputColour = leftTexel * rightTexel * topTexel * bottomTexel;
    */
    
    
    /*
    // store the fragment position vector in the first gbuffer texture
    outPosition = vWorldPosition;
    // also store the per-fragment normals into the gbuffer
    outNormal = normalize(vLocalNormal);
    
    // and the diffuse per-fragment color
    outAlbedoSpec.rgb = texture(texture_diffuse1, vTexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    outAlbedoSpec.a = texture(texture_specular1, vTexCoord).r;
    */
    
    
}
