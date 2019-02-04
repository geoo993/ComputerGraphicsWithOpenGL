#version 410 core

uniform vec3 color;

out vec4 vOutputColour;        // The output colour

void main() {
    
    vOutputColour = vec4(color, 1.0f);
    
}
