#version 400 core

////https://www.youtube.com/watch?v=LyoSSoYyfVU

in vec2 vTexCoord;		
in vec3 vWorldPosition;
in vec3 vLocalNormal;

uniform sampler2D sceneTexture;
uniform sampler2D highlightTexture;
uniform float intensity; 

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    vec4 sceneColour = texture(sceneTexture, vTexCoord);
    vec4 highlightColour = texture(highlightTexture, vTexCoord);
  
    vOutputColour = sceneColour + highlightColour * intensity;

     
}
