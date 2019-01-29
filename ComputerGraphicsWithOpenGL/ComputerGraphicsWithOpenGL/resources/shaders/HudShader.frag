#version 400 core

in vec2 vTexCoord;
out vec4 vOutputColour;

uniform sampler2D textSampler;
uniform vec4 textColor;

void main()
{
	vec4 vTexColour = texture(textSampler, vTexCoord);	// Get the texel colour from the image
	vOutputColour = vec4(vTexColour.r) * textColor;			// The texel colour is a grayscale value -- apply to RGBA and combine with vColor
       
}
