#version 400 core

in vec2 vTexCoord;		
in vec3 vWorldPosition;
in vec3 vLocalNormal;

uniform sampler2D sceneTexture;
uniform sampler2D highlightTexture;
uniform float intensity; 

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    vec4 sceneColour = texture(sceneTexture, vec2(vTexCoord.x, vTexCoord.y));
    vec4 highlightColour = texture(highlightTexture, vec2(vTexCoord.x, vTexCoord.y));
    
    vOutputColour = sceneColour + highlightColour * intensity;

}
