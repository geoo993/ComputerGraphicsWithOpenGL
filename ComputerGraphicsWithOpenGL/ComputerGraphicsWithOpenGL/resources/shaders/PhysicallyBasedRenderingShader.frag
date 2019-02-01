#version 400 core

uniform sampler2D sampler0;
uniform bool bUseTexture;

in VS_OUT
{
    vec2 vTexCoord;    // Texture coordinate
    vec3 vLocalPosition;
    vec3 vLocalNormal;
    vec3 vEyePosition;
    vec3 vEyeNormal;
} fs_in;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    vec4 vTexColour = texture(sampler0, fs_in.vTexCoord);
    vec3 vColour = normalize(fs_in.vLocalNormal);
    
    if (bUseTexture){
        vOutputColour = vTexColour;
    }else{
        vOutputColour = vec4(vColour, 1.0f);
    }
}
