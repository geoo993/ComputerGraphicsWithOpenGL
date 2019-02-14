#version 400 core

in vec2 vTexCoord;	//fbo texture	
in vec3 vWorldPosition;
in vec3 vLocalNormal;

uniform sampler2D sampler0;
uniform bool smoothGradient;
uniform float brightness;

out vec4 vOutputColour;		// The output colour formely  gl_FragColor

void main()
{
    
    vec4 colour = texture(sampler0, vec2(vTexCoord.x, vTexCoord.y));
    
    float b = (colour.r * 0.2126f) + (colour.g * 0.7152f) + (colour.b * 0.0722f);
    
    if (smoothGradient == true){
        vOutputColour = colour * b * b * b;
    }else {
        if (b > brightness){//0.7
            vOutputColour = colour;
        }else {
            vOutputColour = vec4(0.0f);
        }
    }
}
