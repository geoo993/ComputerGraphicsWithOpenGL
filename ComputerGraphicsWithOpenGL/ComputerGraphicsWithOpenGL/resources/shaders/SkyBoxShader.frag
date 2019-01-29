#version 400 core

uniform samplerCube cubeMapTex; // cubemap texture sampler

in vec3 vTextureDirection; // direction vector representing a 3D texture coordinate
in vec2 vTexCoord;

out vec4 vOutputColour;         //formely  gl_FragColor

void main()
{
    
    vOutputColour = texture(cubeMapTex, vTextureDirection);
    
}
